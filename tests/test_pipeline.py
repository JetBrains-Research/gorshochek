from os import path, listdir
import subprocess
import shutil
from typing import List

configs_dir_path = path.join("tests", "configs")
expected_path = path.join("tests", "resources", "expected")
actual_path = path.join("tests", "resources", "actual")
input_path = path.join("tests", "resources", "input")
tricky_path = path.join("scripts", "..", "tests", "..", "tests", "resources", "input")
build = path.join("build", "gorshochek")


def _test(files: List, config_path: str) -> None:
    num_regular = len(files) // 2
    input_files_paths = [
        path.join(input_path, file) for file in files[:-num_regular]
    ] + [
        path.join(tricky_path, file) for file in files[-num_regular:]
    ]
    subprocess.check_call([build, config_path] + input_files_paths)
    assert path.exists(actual_path), f"Transformed files folder \"{actual_path}\" does not exists"

    for file in files:
        file_name, _ = file.rsplit(".")
        expected_file_dir = path.join(expected_path, file_name)
        actual_file_dir = path.join(actual_path, file_name)
        expected_num_transforms = len(listdir(expected_file_dir))
        actual_num_transforms = len(listdir(actual_file_dir))
        assert actual_num_transforms == expected_num_transforms, \
            f"Num transformed files is not equal to num expected"
        for i in range(expected_num_transforms - 1):
            expected_transform_path = path.join(expected_file_dir, f"transformation_{i}.cpp")
            actual_transform_path = path.join(actual_file_dir, f"transformation_{i}.cpp")
            subprocess.check_call(["clang++", actual_transform_path, "-o", path.join(actual_path, f"{i}.o")])
            with open(expected_transform_path, "r") as expected:
                expected_data = expected.read()
            with open(actual_transform_path, "r") as transformed:
                transformed_data = transformed.read()
            assert transformed_data == expected_data, "Actual and expected files mismatch "
    shutil.rmtree(actual_path)


def test_pipeline() -> None:
    config_path = path.join(configs_dir_path, "test_pipeline_config.yaml")
    files = ["test1.cpp", "test2.cpp", "test3.cpp"]
    _test(files, config_path)


def test_remove_comments() -> None:
    config_path = path.join(configs_dir_path, "test_remove_comments_config.yaml")
    files = ["test_remove_comments.cpp"]
    _test(files, config_path)


def test_add_comments() -> None:
    config_path = path.join(configs_dir_path, "test_add_comments_config.yaml")
    files = ["test_add_comments.cpp"]
    _test(files, config_path)


def test_rename_entities() -> None:
    config_path = path.join(configs_dir_path, "test_rename_entities_config.yaml")
    files = ["test_rename_entities.cpp"]
    _test(files, config_path)
