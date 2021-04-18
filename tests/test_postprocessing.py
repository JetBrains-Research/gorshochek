import subprocess
from os import path, listdir, walk

import pytest

data_path = "data"
cf_reduced_data_path = path.join(data_path, "codeforces_reduced")
script_path = path.join("scripts", "postprocessing", "postprocessing.sh")
fdupes_script = ["fdupes", "-r", cf_reduced_data_path]


def test_postprocessing() -> None:
    try:
        subprocess.check_call(["sh", script_path])
    except subprocess.CalledProcessError:
        pytest.fail(f"Running postprocessing script ended with non-zero code")
    assert path.exists(data_path), f"Data path does not exists"
    assert path.exists(cf_reduced_data_path), f"Codeforces reduced does not exists"

    try:
        output = subprocess.check_output(" ".join(fdupes_script), shell=True)
        assert not output.strip()
    except subprocess.CalledProcessError:
        pytest.fail(f"Running fdupes script ended with non-zero code")

    for file in listdir(cf_reduced_data_path):
        if file not in ["duplicates.txt", "clones.txt"]:
            assert path.isdir(path.join(cf_reduced_data_path, file))

    files_number = sum(len(files) for root, dirs, files in walk(cf_reduced_data_path))
    assert files_number
