import pandas as pd
import tensorflow as tf

from typing import List

def make_tf_dataset(
    df: pd.DataFrame,
    targets: List[str],
    val_split: float = 0.1,
    batch_size: int = 2048,
    seed=42,
):
    n_val = round(df.shape[0] * val_split)
    if seed:
        x = df.sample(frac=1, random_state=seed).to_dict("series")
    else:
        x = df.to_dict("series")
    y = dict()
    for t in targets:
        y[t] = x.pop(t)
    ds = tf.data.Dataset.from_tensor_slices((x, y))

    ds_val = ds.take(n_val).batch(batch_size)
    ds_train = ds.skip(n_val).batch(batch_size)
    return ds_train, ds_val