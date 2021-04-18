import shutil
from argparse import ArgumentParser
from os import listdir
from os.path import join, exists


def get_path(file: str, dataset_path: str) -> str:
    file_ = file.rsplit(".", 1)[0]
    file_path = join(dataset_path, file_)
    return file_path


def merge_clones(dataset_path: str, clones_file_path: str):
    with open(clones_file_path, "r") as f:
        clones_text = f.read()

    clones_ = []
    for clones_batch in clones_text.split("\n\n"):
        batch = []
        for file in clones_batch.split("\n"):
            file_path = get_path(file, dataset_path)
            if exists(file_path):
                batch += [file_path]
        if len(batch) > 1:
            clones_ += [batch]

    for batch in clones_:
        base_dir = batch[0]
        for i, file in enumerate(listdir(base_dir)):
            shutil.move(join(base_dir, file), join(base_dir, f"transformation_{i}.cpp"))
        num_default = len(listdir(base_dir))

        files_to_move = []
        for dir_ in batch[1:]:
            files_to_move += [join(dir_, file) for file in listdir(dir_)]

        for i, file in enumerate(files_to_move):
            shutil.move(file, join(base_dir, f"transformation_{i + num_default}.cpp"))


if __name__ == "__main__":
    arg_parser = ArgumentParser()
    arg_parser.add_argument("--dataset_path", type=str, default=join("data", "codeforces_reduced"))
    arg_parser.add_argument("--clones_file_path", type=str, default=join("data", "codeforces_augmented", "clones.txt"))
    args = arg_parser.parse_args()

    merge_clones(dataset_path=args.dataset_path, clones_file_path=args.clones_file_path)
