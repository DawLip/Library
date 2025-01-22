import datetime
import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import tensorflow as tf

import seaborn as sns

def create_interactions_file(dataset="ml-latest-small", target=False, file_name="ratings.csv", folder=""):
  if not target: target = dataset
  os.makedirs(f"./{target}{folder}", exist_ok=True)
  
  interactions = pd.read_csv(f"./{dataset}/{file_name}", names=["user_id", "item_id", "","--"], usecols=[0,1], header=0)
  interactions["interaction"] = 1
  interactions.to_csv(f"./{target}/interaction_data.csv", index=False)
  return interactions

def create_neg_file(interactions, target, folder=""):
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
    os.makedirs(f"./{target}{folder}", exist_ok=True)
    
    unique_items = interactions["item_id"].unique()
    items = pd.DataFrame(unique_items, columns=["item_id"])
    items["new_item_id"] = range(len(items))

    items.to_csv(f"./{target}{folder}/item_id_translation.csv", index=False)
    
    return items

def translate_item_id(interactions, item_id_translation, id_from, id_to, target, folder=""):
    interactions_translated = interactions.merge(
        item_id_translation,
        left_on=id_from,
        right_on="item_id",
        how="left"
    )
    
    interactions_translated[id_from] = interactions_translated[id_to]
    
    interactions_translated = interactions_translated.drop(columns=["item_id"])
    interactions_translated = interactions_translated.rename(columns={"new_item_id": "item_id"})
    
    interactions_translated.to_csv(f"./{target}{folder}/interaction_data.csv", index=False)
    
    return interactions_translated
  
def create_user_ids_file(interactions, target, folder=""):
    os.makedirs(f"./{target}{folder}", exist_ok=True)
    
    unique_items = interactions["user_id"].unique()
    items = pd.DataFrame(unique_items, columns=["user_id"])
    items["new_user_id"] = range(len(items))

    items.to_csv(f"./{target}{folder}/user_id_translation.csv", index=False)
    
    return items

def translate_user_ids(interactions, item_id_translation, id_from, id_to, target, folder=""):
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