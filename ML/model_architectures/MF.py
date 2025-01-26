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

import optuna

def create_model_MF(p):
  user_input = Input(shape=(1,), dtype='int32', name = 'user_input')
  item_input = Input(shape=(1,), dtype='int32', name = 'item_input')

  MF_Embedding_User = Embedding(
    input_dim = p["n_users"], output_dim = p["latent_dim"], 
    name = 'user_embedding',
    embeddings_initializer="RandomNormal", embeddings_regularizer = l2(p["embeding_l2"]), 
    input_length=1
  )
  MF_Embedding_Item = Embedding(
    input_dim = p["n_items"], output_dim = p["latent_dim"], 
    name = 'item_embedding',
    embeddings_initializer="RandomNormal", embeddings_regularizer = l2(p["embeding_l2"]), 
    input_length=1
  )   
  
  user_latent = Flatten()(MF_Embedding_User(user_input))
  item_latent = Flatten()(MF_Embedding_Item(item_input))
  
  predict_vector = Concatenate(name="GMF_Layer")([user_latent, item_latent])
  prediction = Dense(1, activation='sigmoid', kernel_initializer='lecun_uniform', name = 'prediction')(predict_vector)
  
  return Model(inputs=[user_input, item_input], outputs=prediction)