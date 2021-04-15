import shutil
from os import remove, listdir
from os.path import join

dups_path = "dups.txt"

def merge_clones():
    with open(dups_path, "r") as f:
        dups = f.read()

    # Leaving single file
    dups = [dup.split("\n")[1:] for dup in dups.split("\n\n") if dup]
    for dup_ in dups:
        for file in dup_:
            remove(file)

    for dir_ in listdir("actual_reduced"):
        dir_path = join("actual_reduced", dir_)
        if len(listdir(dir_path)) == 1:
            shutil.rmtree(dir_path)

if __name__ == "__main__":
    arg_parser = ArgumentParser()
    arg_parser.add_argument("--dataset_path", type=str, default=join("data", "codeforces"))
    arg_parser.add_argument("--duplicates_file", type=str, default="duplicates.txt")
    args = arg_parser.parse_args()

    process_duplicates_file(dataset_path=args.dataset_path, duplicates_file=args.duplicates_file)
