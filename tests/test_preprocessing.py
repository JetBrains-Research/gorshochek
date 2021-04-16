import zipfile
import shutil
import subprocess
from os import path, listdir

import pytest

script_path = path.join("scripts", "codeforces", "download_data.sh")
data_path = "data"
cf_zip_path = path.join(data_path, "codeforces.zip")
cf_data_path = path.join(data_path, "codeforces")
duplicates_file_path = path.join(cf_data_path, "clones.txt")
tmp_data_path = path.join(data_path, "anti-plagiarism-datasets-master")
rounds_storage_path = path.join(tmp_data_path, "rounds")


def test_downloading() -> None:
    try:
        subprocess.check_call(["sh", script_path])
    except subprocess.CalledProcessError:
        pytest.fail(f"Running downloading script ended with non-zero code")
    assert path.exists(data_path), f"Data path does not exists"
    assert path.exists(cf_zip_path), f"Codeforces zip was not downloaded"
    assert path.exists(cf_data_path), f"Codeforces was not unzipped"
    assert path.exists(duplicates_file_path), f"Codeforces clones does not exists"

    with zipfile.ZipFile(cf_zip_path, "r") as zip_:
        zip_.extractall(data_path)

    rounds = listdir(rounds_storage_path)
    for round_ in rounds:
        round_zip_path = path.join(rounds_storage_path, round_)
        with zipfile.ZipFile(round_zip_path, "r") as zip_:
            zip_.extractall(rounds_storage_path)
        round_path = round_zip_path.rsplit(".", 1)[0]
        for task in listdir(round_path):
            assert path.exists(path.join(cf_data_path, task)), f"Task {task} does not exists in preprocessed dataset"

    for task in listdir(cf_data_path):
        task_path = path.join(cf_data_path, task)
        if path.isdir(task_path):
            for file in listdir(task_path):
                ext = file.rsplit(".", 1)[1]
                assert ext in ["cpp", "c"]

    shutil.rmtree(data_path)
