import shutil
from os import remove, listdir
from os.path import join

dups_path = "dups.txt"

if __name__ == "__main__":
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