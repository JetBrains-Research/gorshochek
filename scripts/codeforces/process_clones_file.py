from argparse import ArgumentParser
from os.path import join, exists


def process_duplicates_file(dataset_path: str, clones_file: str = "clones.txt"):
    duplicates_path = join(dataset_path, clones_file)
    with open(duplicates_path, "r") as f:
        duplicates_text = f.read().strip()

    filtered_duplicates = []
    for duplicates_batch in duplicates_text.split("\n\n"):
        rows = duplicates_batch.strip().split("\n")
        task, duplicates_rows = rows[0], rows[2:]
        for row in duplicates_rows:
            row_duplicates = []
            for file in row.split(" "):
                full_path = join(dataset_path, task, file)
                ext = file.rsplit(".", 1)[1]
                if ext in ["cpp", "c"]:
                    if not exists(full_path):
                        raise ValueError(f"File {file} in task {task} does not exists")
                    row_duplicates += [file]
            if len(row_duplicates) > 1:
                filtered_duplicates.append("\n".join(row_duplicates))

    with open(duplicates_path, "w") as f:
        f.write("\n\n".join(filtered_duplicates))


if __name__ == "__main__":
    arg_parser = ArgumentParser()
    arg_parser.add_argument("--dataset_path", type=str, default=join("data", "codeforces"))
    arg_parser.add_argument("--clones_file", type=str, default="clones.txt")
    args = arg_parser.parse_args()

    process_duplicates_file(dataset_path=args.dataset_path, clones_file=args.clones_file)
