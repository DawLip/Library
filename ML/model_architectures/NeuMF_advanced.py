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

def create_model_NeuMF_advanced(p):
  layers_mlp=[{"neurons":p["mlp_layers_neurons"]*2**x, "l2":p["mlp_l2"]} for x in range(p["mlp_layers_num"])]
  layers_mlp.reverse()
  
  layers_mlp2=[{"neurons":p["mlp2_layers_neurons"]*2**x} for x in range(p["mlp2_layers_num"])]
  layers_mlp2.reverse()
    
  def create_mlp(user, item):
    mlp_user_embedding = Embedding(
      input_dim=p["n_users"], output_dim=p["latent_dim_mlp"],
      name="MLP_User_Embedding",
      embeddings_initializer="RandomNormal", embeddings_regularizer = l2(p["embeding_l2_mlp"]),
      input_length=1,
    )
    
    mlp_item_embedding = Embedding(
      input_dim=p["n_items"], output_dim=p["latent_dim_mlp"],
      name="MLP_Item_Embedding",
      embeddings_initializer="RandomNormal", embeddings_regularizer = l2(p["embeding_l2_mlp"]),
      input_length=1,
    )
    
    mlp_user_vector = Flatten(name="MLP_User_Vector")(mlp_user_embedding(user))
    mlp_item_vector = Flatten(name="MLP_Item_Vector")(mlp_item_embedding(item))

    mlp_vector = Concatenate(name="MLP_Concatenation")([mlp_user_vector, mlp_item_vector])
    
    for i in range(len(layers_mlp)):
      layer = Dense(
        layers_mlp[i]["neurons"],
        activity_regularizer=l2(layers_mlp[i]["l2"]),
        activation="relu",
        name=f"MLP_Layer_{i+1}",
      )
      mlp_vector = layer(mlp_vector)
        
    return mlp_vector
  
  def create_gmf(user, item):
    mf_user_embedding = Embedding(
      input_dim=p["n_users"],
      output_dim=p["latent_dim_mf"],
      name="MF_User_Embedding",
      embeddings_initializer="RandomNormal",
      embeddings_regularizer=l2(p["embeding_l2_mf"]),
      input_length=1,
    )
    
    mf_item_embedding = Embedding(
      input_dim=p["n_items"],
      output_dim=p["latent_dim_mf"],
      name="MF_Item_Embedding",
      embeddings_initializer="RandomNormal",
      embeddings_regularizer=l2(p["embeding_l2_mf"]),
      input_length=1,
    )

    mf_user_vector = Flatten(name="MF_User_Vector")(mf_user_embedding(user))
    mf_item_vector = Flatten(name="MS_Item_Vector")(mf_item_embedding(item))
    
    return Multiply(name="GMF_Layer")([mf_user_vector, mf_item_vector])

  user = Input(shape=(), dtype="int32", name="user_id")
  item = Input(shape=(), dtype="int32", name="item_id")

  neumf_layer = Concatenate(name="NeuMF_Layer")([create_gmf(user, item), create_mlp(user, item)])
  
  mlp_vector = neumf_layer
  for i in range(len(layers_mlp2)):
      layer = Dense(
        layers_mlp2[i]["neurons"],
        activation="relu",
        name=f"MLP2_Layer_{i+1}",
      )
      mlp_vector = layer(mlp_vector)
  
  output = Dense(1, activation="sigmoid", kernel_initializer="lecun_uniform", name="interaction")(mlp_vector)

  return Model(inputs=[user, item], outputs=[output])