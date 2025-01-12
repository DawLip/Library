import sys
import os

import numpy as np
import pandas as pd

from tensorflow.keras.models import load_model

from make_tf_dataset import make_tf_dataset

project_name = sys.argv[1]
user_id = int(sys.argv[2])

savesFolder = f"./app/saves/{project_name}"
# movielens_small_dataset = "./ML/ml-latest-small"

items = pd.read_csv(f"{savesFolder}/borrowed_books.csv", sep=",", names=["user_id", "item_id","1","2"], usecols=[0,1])
items = items[items["user_id"] == user_id]["item_id"]
books = pd.read_csv(f"{savesFolder}/books.csv", sep=",", names=["item_id", " ", "1","2","3","4"], usecols=[0])

model = load_model(f"{savesFolder}/model.keras")

data = pd.DataFrame({
  "user_id": np.full(len(items), user_id),
  "item_id": items,
  "interaction": np.ones(shape=(len(items),))
})

data = pd.merge(books, data, on="item_id", how="outer")
data['user_id'] = data['user_id'].fillna(user_id)
data['interaction'] = data['interaction'].fillna(0)

dataset, _ = make_tf_dataset(data, ["interaction"], val_split=0, seed=None)

data["predictions"] = model.predict(dataset) 
data = data[data["interaction"]==0].sort_values("predictions", ascending=False)[:5]

print("RETURN:")
print(" ".join(map(str, data["item_id"].values)))