import datetime
import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import tensorflow as tf

import seaborn as sns

def create_interactions_file(dataset="ml-latest-small", target=False, file_name="ratings.csv", folder=""):
  """
  Creates interaction data file from given dataset.

  Parameters
  ----------
  dataset : str
    Name of the dataset, e.g. ml-latest-small
  target : str
    Name of the target folder, if not specified, the same as dataset
  file_name : str
    Name of the file containing the data, e.g. ratings.csv
  folder : str
    Additional folder to create, e.g. /datasets

  Returns
  -------
  interactions : pandas.DataFrame
    DataFrame containing user-item interactions
  """
  if not target: target = dataset
  os.makedirs(f"./{target}{folder}", exist_ok=True)
  
  interactions = pd.read_csv(f"./{dataset}/{file_name}", names=["user_id", "item_id", "","--"], usecols=[0,1], header=0)
  interactions["interaction"] = 1
  interactions.to_csv(f"./{target}/interaction_data.csv", index=False)
  return interactions

def create_neg_file(interactions, target, folder=""):
  """
  Generates a CSV file containing negative samples for each user.

  Parameters
  ----------
  interactions : pandas.DataFrame
      DataFrame containing user-item interactions with columns ['user_id', 'item_id'].
  target : str
      The directory where the negative samples file will be saved.
  folder : str, optional
      Additional subfolder path within the target directory, by default "".

  Returns
  -------
  pandas.DataFrame
      DataFrame containing negative samples with columns ['user_id', 'item_ids'].

  The function creates a CSV file named 'negative_samples.csv' in the specified
  target directory. For each user, it identifies items they have not interacted with
  and stores these as potential negative samples.
  """

  os.makedirs(f"./{target}{folder}", exist_ok=True)
  
  users = interactions['user_id'].unique()
  items = interactions['item_id'].unique()
  
  negative_samples = []
  for user in users:
    user_interacted_items = interactions[interactions['user_id'] == user]['item_id'].tolist()
    potential_neg_items = np.setdiff1d(items, user_interacted_items, assume_unique=True)
    
    negative_samples.append({'user_id': user, 'item_ids': potential_neg_items})
    
  negative_df = pd.DataFrame(negative_samples)
  negative_df.to_csv(f"./{target}{folder}/negative_samples.csv", index=False)
    
  return negative_df

def generate_negative_samples(user_id, negative, num_negatives=3):
    # Losowy wybór 10 unikalnych item_id
    negative_items = np.random.choice(np.concatenate(negative[negative["user_id"]==3]["item_ids"].values), size=num_negatives)
    
    # Tworzenie DataFrame z negatywnymi próbkami
    negative_samples = pd.DataFrame({
        'user_id': [user_id] * num_negatives,
        'item_id': negative_items,
        'interaction': [0] * num_negatives
    })
    
    return negative_samples
  
def create_train_test_files(interactions, negative, target, seed=43, train_negatives_num=1, test_negatives_num=100, folder=""):
  """
  Creates train and test files for a given dataset.

  Parameters
  ----------
  interactions : pandas.DataFrame
    DataFrame containing user-item interactions with columns ['user_id', 'item_id', 'interaction'].
  negative : pandas.DataFrame
    DataFrame containing negative samples for each user with columns ['user_id', 'item_ids'].
  target : str
    The directory where the train and test files will be saved.
  seed : int, optional
    The seed for the random sampling, by default 43.
  train_negatives_num : int, optional
    The number of negative samples to add to the training data for each user, by default 1.
  test_negatives_num : int, optional
    The number of negative samples to add to the test data for each user, by default 100.
  folder : str, optional
    Additional subfolder path within the target directory, by default "".

  Returns
  -------
  train : pandas.DataFrame
    DataFrame containing the training data with columns ['user_id', 'item_id', 'interaction'].
  test : pandas.DataFrame
    DataFrame containing the test data with columns ['user_id', 'item_id', 'interaction'].
  """
  os.makedirs(f"./{target}{folder}", exist_ok=True)
  
  unique_users = interactions['user_id'].unique()
  
  df = interactions.sample(frac=1).sort_values(by=['user_id']).reset_index(drop=True)

  test = df.groupby('user_id').tail(1).copy()
  train = df.drop(test.index).copy()

  train_negatives = pd.concat([generate_negative_samples(user_id, negative, train_negatives_num*len(df[df["user_id"]==user_id])) for user_id in unique_users])
  train = pd.concat([train, train_negatives]).sample(frac=1)
  
  test_negatives = pd.concat([generate_negative_samples(user_id, negative, test_negatives_num) for user_id in unique_users])
  test = pd.concat([test, test_negatives]).sample(frac=1)
  
  train.to_csv(f"./{target}{folder}/train_data_1-{train_negatives_num}.csv", index=False)
  test.to_csv(f"./{target}{folder}/test_data.csv", index=False)
  
  return train, test

def create_items_ids_file(interactions, target, folder=""):
    """
    Creates a CSV file containing a translation of item IDs to new item IDs.
    
    Parameters
    ----------
    interactions : pandas.DataFrame
        DataFrame containing user-item interactions with columns ['user_id', 'item_id', 'interaction'].
    target : str
        The directory where the item ID translation file will be saved.
    folder : str, optional
        Additional subfolder path within the target directory, by default "".
    
    Returns
    -------
    pandas.DataFrame
        DataFrame containing the item ID translation with columns ['item_id', 'new_item_id'].
    """
    os.makedirs(f"./{target}{folder}", exist_ok=True)
    
    unique_items = interactions["item_id"].unique()
    items = pd.DataFrame(unique_items, columns=["item_id"])
    items["new_item_id"] = range(len(items))

    items.to_csv(f"./{target}{folder}/item_id_translation.csv", index=False)
    
    return items

def translate_item_id(interactions, item_id_translation, id_from, id_to, target="", folder="", save=True):
    """
    Translates the item IDs in the given interactions DataFrame using the given translation table.
    
    Parameters
    ----------
    interactions : pandas.DataFrame
        DataFrame containing user-item interactions with columns ['user_id', 'item_id', 'interaction'].
    item_id_translation : pandas.DataFrame
        DataFrame containing the item ID translation with columns ['item_id', 'new_item_id'].
    id_from : str
        The name of the column in interactions to translate from.
    id_to : str
        The name of the column in item_id_translation to translate to.
    target : str, optional
        The directory where the translated interactions will be saved, by default "".
    folder : str, optional
        Additional subfolder path within the target directory, by default "".
    save : bool, optional
        Whether to save the translated interactions to a file, by default True.
    
    Returns
    -------
    pandas.DataFrame
        DataFrame containing the translated interactions with columns ['user_id', 'item_id', 'interaction'].
    """
    interactions_translated = interactions.merge(
        item_id_translation,
        left_on=id_from,
        right_on="item_id",
        how="left"
    )
    
    interactions_translated[id_from] = interactions_translated[id_to]
    
    interactions_translated = interactions_translated.drop(columns=["item_id"])
    interactions_translated = interactions_translated.rename(columns={"new_item_id": "item_id"})
    
    if save: interactions_translated.to_csv(f"./{target}{folder}/interaction_data.csv", index=False)
    
    return interactions_translated
  
def create_user_ids_file(interactions, target, folder=""):
    """
    Creates a CSV file containing a translation of user IDs to new user IDs.

    Parameters
    ----------
    interactions : pandas.DataFrame
        DataFrame containing user-item interactions with columns ['user_id', 'item_id', 'interaction'].
    target : str
        The directory where the user ID translation file will be saved.
    folder : str, optional
        Additional subfolder path within the target directory, by default "".

    Returns
    -------
    pandas.DataFrame
        DataFrame containing the user ID translation with columns ['user_id', 'new_user_id'].
    """
    os.makedirs(f"./{target}{folder}", exist_ok=True)
    
    unique_items = interactions["user_id"].unique()
    items = pd.DataFrame(unique_items, columns=["user_id"])
    items["new_user_id"] = range(len(items))

    items.to_csv(f"./{target}{folder}/user_id_translation.csv", index=False)
    
    return items

def translate_user_ids(interactions, item_id_translation, id_from, id_to, target, folder=""):
  """
  Translates the user IDs in the given interactions DataFrame using the given translation table.

  Parameters
  ----------
  interactions : pandas.DataFrame
      DataFrame containing user-item interactions with columns ['user_id', 'item_id', 'interaction'].
  item_id_translation : pandas.DataFrame
      DataFrame containing the user ID translation with columns ['user_id', 'new_user_id'].
  id_from : str
      The name of the column in interactions to translate from.
  id_to : str
      The name of the column in item_id_translation to translate to.
  target : str
      The directory where the translated interactions will be saved.
  folder : str, optional
      Additional subfolder path within the target directory, by default "".

  Returns
  -------
  pandas.DataFrame
      DataFrame containing the translated interactions with columns ['user_id', 'item_id', 'interaction'].
  """

  interactions_translated = interactions.merge(
    item_id_translation,
    left_on=id_from,
    right_on="user_id",
    how="left"
  )
  
  interactions_translated[id_from] = interactions_translated[id_to]
  
  interactions_translated = interactions_translated.drop(columns=["user_id"])
  interactions_translated = interactions_translated.rename(columns={"new_user_id": "user_id"})
  
  interactions_translated.to_csv(f"./{target}{folder}/interaction_data.csv", index=False)
  
  return interactions_translated
  
def parse_dataset(target):
  """
  Creates interaction data, item ID translation, user ID translation, negative samples and train/test files for a given dataset.

  Parameters
  ----------
  target : str
      The name of the dataset, e.g. ml-latest-small

  Returns
  -------
  None
  """
  interactions = create_interactions_file(dataset=target, file_name="borrowed_books.csv", folder="/datasets")
  interactions.drop_duplicates()
  item_id_translation = create_items_ids_file(interactions, target=target, folder="/datasets")
  interactions = translate_item_id(interactions, item_id_translation, "item_id", "new_item_id", target=target, folder="/datasets")
  print(interactions)
  user_id_translation = create_user_ids_file(interactions, target=target)
  interactions = translate_user_ids(interactions, user_id_translation, "user_id", "new_user_id", target=target, folder="/datasets")

  negative = create_neg_file(interactions, target, folder="/datasets")
  create_train_test_files(interactions, negative, target=target, train_negatives_num=10, test_negatives_num=100, folder="/datasets")
  
# parse_dataset("app/saves/MovieLens_s_dataset_50")