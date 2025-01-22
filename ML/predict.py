import sys
import os

import numpy as np
import pandas as pd

from tensorflow.keras.models import load_model

from make_tf_dataset import make_tf_dataset
from utils import *
from dataset_utils import *

project_name = sys.argv[1]
model_name = sys.argv[2]
user_id = int(sys.argv[3])

savesFolder = f"./app/saves/{project_name}/"

interactions = pd.read_csv(f"{savesFolder}/datasets/interaction_data.csv", sep=",", usecols=[0,1,2])
item_id_translation = pd.read_csv(f"{savesFolder}/datasets/item_id_translation.csv", sep=",", usecols=[0,1])
items_all = interactions["item_id"].unique()

model = load_model(f"{savesFolder}/{model_name}/{model_name}.keras")

data = pd.DataFrame({
  "user_id": np.full(len(items_all), user_id),
  "item_id": items_all,
})

data = pd.merge(data, interactions, on=["item_id", "user_id"], how="left")
data['interaction'] = data['interaction'].fillna(0)

data["predictions"] = model.predict([data["user_id"].values, data["item_id"].values], batch_size=1024*8, verbose=0)
data = data[data["interaction"]==0].sort_values("predictions", ascending=False)[:5]

data = data.rename(columns={"item_id": "new_item_id"})
data = data.merge(
  item_id_translation,
  left_on="new_item_id",
  right_on="new_item_id",
  how="left"
)

print("RETURN:")
print(" ".join(map(str, data["item_id"].values)))