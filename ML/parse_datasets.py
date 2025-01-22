from dataset_utils import *

def parse_dataset(target):
  interactions = create_interactions_file(dataset=target, file_name="borrowed_books.csv", folder="/datasets")
  item_id_translation = create_items_ids_file(interactions, target=target, folder="/datasets")
  interactions = translate_item_id(interactions, item_id_translation, "item_id", "new_item_id", target=target, folder="/datasets")

  user_id_translation = create_user_ids_file(interactions, target=target)
  interactions = translate_user_ids(interactions, user_id_translation, "user_id", "new_user_id", target=target, folder="/datasets")

  negative = create_neg_file(interactions, target, folder="/datasets")
  create_train_test_files(interactions, negative, target=target, train_negatives_num=10, test_negatives_num=100, folder="/datasets")
  
# parse_dataset("app/saves/MovieLens_s_dataset_50")