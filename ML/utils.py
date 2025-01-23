import datetime
import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import tensorflow as tf

import seaborn as sns
from sklearn.metrics import ndcg_score

from keras.initializers import RandomNormal
import tensorflow as tf
import tensorflow.keras as keras
from tensorflow.keras.layers import (
    Concatenate,
    Dense,
    Embedding,
    Flatten,
    Input,
    Multiply,
)
from tensorflow.keras.models import Model
from tensorflow.keras.regularizers import l2

def make_metrics_vs_epochs_graphs(train_hist, callback_hist, save=False, path_to_save=""):
  """
  Function to generate a graph of metrics vs. epochs from a given Keras `History` object.
  
  Parameters:
  train_hist (History): The `History` object from a fit operation.
  callback_hist (History): A `History` object from a callback.
  save (bool): Whether to save the graph or display it.
  path_to_save (str): Path to save the graph to.
  
  Returns:
  None
  """
  metrics=["auc", "precision", "recall"]
  plt.figure(figsize=(len(metrics)*6, 10))
  
  def plot(name, x, s1, s2, s3):
    """
    Plot a given metric vs. epochs.

    Parameters:
    name (str): Name of the metric to plot.
    x (list): The values of the metric to plot.
    s1 (int): The number of rows in the subplot layout.
    s2 (int): The number of columns in the subplot layout.
    s3 (int): The index of the subplot in the layout.

    Returns:
    None
    """
    plt.subplot(s1, s2, s3)
    plt.plot(x, label=f'Training {name}', marker='o')
    plt.title(f"{name} vs. Epochs")
    plt.xlabel("Epochs")
    plt.ylabel(name)
    plt.legend()
    plt.grid(True)

  plot("loss", train_hist.history["loss"], 2, len(metrics), 1)
  plot("NDGC@10", callback_hist.NDGCat10, 2, len(metrics), 2)
  plot("HR@10", callback_hist.HRat10, 2, len(metrics), 3)
  
  for i, m in enumerate(metrics):
    plot(m, train_hist.history[m], 2, len(metrics), i+4)

  if not save:  plt.show()
  else: plt.savefig(f"{path_to_save}/metrics_vs_epochs.png")
  
def calculate_hr_at_k(test_data, k=10, target_col="interaction", recs_col="predictions"):
  """
  Calculate Hit Rate at K metric for a given dataset.

  Parameters:
  test_data (pandas.DataFrame): The test data to calculate the metric for.
  k (int): The number of items to consider for calculating the metric.
  target_col (str): The column name of the target variable.
  recs_col (str): The column name of the recommendations.

  Returns:
  float: The calculated Hit Rate at K metric.
  """
  test_data = test_data.sort_values(["user_id", "predictions"], ascending=[True, False])
  top_k_items = test_data.groupby("user_id").head(k)
  hr_at_k = top_k_items.groupby("user_id")["interaction"].max().mean()

  return hr_at_k

class EarlyStoppingAtNDGC(tf.keras.callbacks.Callback):
  bestNDCG=0
  counter=0
  def __init__(self, patience, test_data):
    super().__init__()
    self.patience = patience
    self.test_data = test_data
    self.NDGCat10 = []
    self.HRat10 = []
    self.valid_true = []
    self.y_score = []
    
  def on_epoch_end(self, epoch, logs=None):
    test_data = self.test_data
    
    predictions = self.model.predict([test_data["user_id"].values, test_data["item_id"].values], batch_size=1024*8, verbose=0)
    test_data["predictions"] = predictions  

    td2 = test_data.groupby(['user_id', 'item_id', 'predictions'], as_index=False)['interaction'].max()
    self.y_score = td2.pivot(index='user_id', columns='item_id', values='predictions').fillna(0).values
    self.valid_true = td2[['user_id', 'item_id', 'interaction']].pivot(index='user_id', columns='item_id', values='interaction').fillna(0).values

    ndcg_at_k = ndcg_score(self.valid_true, self.y_score, k=10)
    hr_at_k = calculate_hr_at_k(test_data, k=10)
    
    logs["NDGC@10"] = ndcg_at_k
    self.NDGCat10.append(ndcg_at_k)
    
    logs["HR@10"] = hr_at_k
    self.HRat10.append(hr_at_k)
    
    epsylon=0.001
    if(self.bestNDCG+epsylon<ndcg_at_k):
      self.bestNDCG = ndcg_at_k
      self.wait = 0
    else: self.wait += 1
    
    if self.wait >= self.patience:
      print(f"Epoch {epoch+1}: Early stopping triggered.")
      self.stopped_epoch = epoch
      self.model.stop_training = True
      
def make_metrics_vs_k_graphs(data, save=False, path_to_save=""):
  """
  Plot two subplots with NDGC@K and HR@K metrics for a given dataset.

  Parameters:
  data (pandas.DataFrame): The dataset to calculate the metrics for.
  save (bool): Whether to save the plot to a file. If True, path_to_save must be specified.
  path_to_save (str): The path to save the plot to. If not specified, the plot will be shown in a window.

  Returns:
  None
  """
  def at_k(K=10):
    ndcg=[]
    hr=[]
		
    for i in range(K):
      td2 = data.groupby(['user_id', 'item_id', 'predictions'], as_index=False)['interaction'].max()
      y_score = td2.pivot(index='user_id', columns='item_id', values='predictions').fillna(0).values
      valid_true = td2[['user_id', 'item_id', 'interaction']].pivot(index='user_id', columns='item_id', values='interaction').fillna(0).values
      TOP_K=i+1
      ndcg.append(ndcg_score(valid_true, y_score, k=TOP_K))
      hr.append(calculate_hr_at_k(data, k=TOP_K))
    return ndcg, hr
    
  ndcg, hr = at_k()

  plt.figure(figsize=(8, 4))

  for i, m in enumerate([[ndcg, "NDGC"],[hr, "HR"],]):
    plt.subplot(1, 2, i+1)
    plt.plot(range(1,11), m[0], marker='o')
    plt.xlabel("K")
    plt.ylabel(f"{m[1]}@K")
    plt.ylim(0, 1)
    plt.grid(True)
  
  if not save:  plt.show()
  else: plt.savefig(f"{path_to_save}/metrics_vs_k.png")
  
def load_dataset(dataset_path = "./datasets/ml-latest-small"):
  """
  Loads a dataset from given path.

  Parameters
  ----------
  dataset_path : str
      The path to the dataset.

  Returns
  -------
  train_data : pandas.DataFrame
      The training data.
  test_data : pandas.DataFrame
      The test data.
  users_all : numpy.ndarray
      All user IDs.
  items_all : numpy.ndarray
      All item IDs.
  """
  interactions_path = f"{dataset_path}/interaction_data.csv"
  negative_samples_path = f"{dataset_path}/negative_samples.csv"
  train_data_path = f"{dataset_path}/train_data_1-10.csv"
  test_data_path = f"{dataset_path}/test_data.csv"


  interactions = pd.read_csv(interactions_path)
  negative_samples = pd.read_csv(negative_samples_path)
  train_data = pd.read_csv(train_data_path)
  test_data = pd.read_csv(test_data_path)

  users_all = interactions["user_id"].unique()
  items_all = interactions["item_id"].unique()
  
  return train_data, test_data, users_all, items_all
  
def to_dataset(data, batch_size=2048*8, test=False):
  """
  Converts a pandas DataFrame into a TensorFlow dataset.

  Parameters
  ----------
  data : pandas.DataFrame
      The data to be converted, containing columns 'user_id', 'item_id', and optionally 'interaction'.
  batch_size : int, optional
      The size of the batches in the resulting dataset, by default 2048*8.
  test : bool, optional
      Flag indicating whether the dataset is for testing. If True, 'interaction' column is not included, by default False.

  Returns
  -------
  tf.data.Dataset
      A TensorFlow dataset with user and item IDs, and optionally interactions if not in test mode.
  """

  user_ids = tf.convert_to_tensor(data["user_id"].values, dtype=tf.int64)
  item_ids = tf.convert_to_tensor(data["item_id"].values, dtype=tf.int64)
  
  if not test:
    interactions = tf.convert_to_tensor(data["interaction"].values, dtype=tf.int64)

    dataset = tf.data.Dataset.from_tensor_slices(((user_ids, item_ids), interactions))
    dataset = dataset.shuffle(buffer_size=2048).batch(batch_size).prefetch(tf.data.AUTOTUNE)
    return dataset

  return tf.data.Dataset.from_tensor_slices((user_ids, item_ids))