import datetime
import os
import json
import sys

import tensorflow as tf

import tensorflow as tf
import tensorflow.keras as keras

from utils import *
from dataset_utils import *
from model_architectures.NeuMF import *

def train_model(project_name, model_name, train_dataset_name, path_to_project):
  os.makedirs(f"./{path_to_project}/{project_name}/{model_name}", exist_ok=True)

  parse_dataset(f"app/saves/{project_name}")
  
  train_data, test_data, users_all, items_all = load_dataset(dataset_path=f"./app/saves/{project_name}/datasets")
  train_dataset = to_dataset(train_data, batch_size=2048)
  

  with open(f"./ML/model_architectures/{model_name}_hiper_params.json", "r") as json_file:
    hiper_params = json_file.read()
    
  hiper_params = json.loads(hiper_params)

  hiper_params["n_users"] = len(users_all)
  hiper_params["n_items"] = len(items_all)
  model = create_model_NeuMF(hiper_params)

  model.compile(
    optimizer="adam",
    loss="binary_crossentropy",
    metrics=[
      tf.keras.metrics.BinaryAccuracy(name="accuracy"),
      tf.keras.metrics.AUC(name="auc"),
      tf.keras.metrics.Precision(name="precision"),
      tf.keras.metrics.Recall(name="recall"),
      tf.keras.metrics.TruePositives(name="tp"),
      tf.keras.metrics.FalsePositives(name="fp"),
      tf.keras.metrics.TrueNegatives(name="tn"),
      tf.keras.metrics.FalseNegatives(name="fn"),
    ],
  )

  logdir = os.path.join("logs", datetime.datetime.now().strftime("%Y%m%d-%H%M%S"))
  earlyStoppingAtNDGC = EarlyStoppingAtNDGC(patience=60, test_data=test_data)

  train_hist = model.fit(
    train_dataset,
    epochs=10,
    callbacks=[tf.keras.callbacks.TensorBoard(logdir, histogram_freq=1), earlyStoppingAtNDGC],
    verbose=1
  ) 

  path_to_save = f"./{path_to_project}/{project_name}/{model_name}"
  
  make_metrics_vs_epochs_graphs(train_hist, earlyStoppingAtNDGC, save=True, path_to_save=path_to_save)
  make_metrics_vs_k_graphs(test_data, save=True, path_to_save=path_to_save)
  model.save(f"{path_to_save}/{model_name}.keras")
  
if __name__ == "__main__":  
  project_name = sys.argv[1]
  model_name = sys.argv[2]
  
  train_model(project_name, model_name, project_name, "./app/saves")
