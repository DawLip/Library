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
    """
    Create a tensorflow dataset from a pandas DataFrame.

    Args:
        df (pd.DataFrame): A pandas DataFrame
        targets (List[str]): A list of column names to be used as targets
        val_split (float, optional): The proportion of the data to be used for validation. Defaults to 0.1.
        batch_size (int, optional): The batch size for the dataset. Defaults to 2048.
        seed (int, optional): The random seed to use for shuffling the data. Defaults to 42.

    Returns:
        Tuple[tf.data.Dataset, tf.data.Dataset]: A tuple of two tensorflow datasets, the first is the training set and the second is the validation set
    """
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