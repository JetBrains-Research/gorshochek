import shutil
from argparse import ArgumentParser
from os import remove, listdir
from os.path import join, exists, dirname


def get_path(file: str, dataset_path: str) -> str:
    file_ = file.rsplit(".", 1)[0]
    file_path = join(dataset_path, file_)
    return file_path


def delete_duplicates(duplicates_file_path: str):
    with open(duplicates_file_path, "r") as f:
        duplicates_text = f.read().strip()

    duplicates = []
    for duplicates_batch in duplicates_text.split("\n\n"):
        duplicates += [duplicates_batch.split("\n")]

    for batch in duplicates:
        parent_dirs = [dirname(file) for file in batch]
        unique_parents = list(set(parent_dirs))
        if len(unique_parents) == 1:
            # Remove all duplicates which came from the same origin
            parent_dir = dirname(batch[0])
            # Remove dir if all files are not duplicates of each other
            if len(listdir(parent_dir)) < len(batch):
                # Leave only single file
                for file in batch[1:]:
                    if exists(file):
                        remove(file)
        else:
            # Remove duplicates came from different origins,
            # leaving only the first one. Later we will move all
            # transformed files which are not duplicates to the dir
            # corresponding to the file which was not deleted
            for file in batch[1:]:
                if exists(file):
                    remove(file)

    for batch in duplicates:
        # Some duplicates came from different origins
        parent_dirs = [dirname(file) for file in batch]
        unique_parents = list(set(parent_dirs))
        if len(unique_parents) > 1:
            base_dir = parent_dirs[0]
            for i, file in enumerate(listdir(base_dir)):
                shutil.move(join(base_dir, file), join(base_dir, f"transformation_{i}.cpp"))
            num_default = len(listdir(base_dir))
            files_to_move = []
            # Collect the remaining files from the dirs where duplicates were found
            for parent in unique_parents:
                if listdir(parent):
                    files_to_move += [join(parent, file) for file in listdir(parent)]

            for i, file in enumerate(files_to_move):
                shutil.move(file, join(base_dir, f"transformation_{i + num_default}.cpp"))


if __name__ == "__main__":
    arg_parser = ArgumentParser()
    arg_parser.add_argument(
        "--duplicates_file_path",
        type=str,
        default=join("data", "codeforces_augmented", "duplicates.txt")
    )
    args = arg_parser.parse_args()

    delete_duplicates(duplicates_file_path=args.duplicates_file_path)
