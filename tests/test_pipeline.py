import inspect
import shutil
import subprocess
from os import path, listdir
from typing import List

import pytest

configs_dir_path = path.join("tests", "configs")
logs_file = path.join("tests", "clang_logs.txt")
expected_path = path.join("tests", "resources", "expected")
actual_path = path.join("tests", "resources", "actual")
input_path = path.join("tests", "resources", "input")
clang_path = path.join("build", "clang+llvm-9", "bin", "clang++")
tricky_path = path.join("scripts", "..", "tests", "..", "tests", "resources", "input")
build = path.join("build", "gorshochek")


def _test(files: List, config_path: str) -> None:
    # Getting the caller function name
    test_name = inspect.stack()[1].function
    num_regular = len(files) // 2
    input_files_paths = [
                            path.join(input_path, file) for file in files[:-num_regular]
                        ] + [
                            path.join(tricky_path, file) for file in files[-num_regular:]
                        ]
    try:
        subprocess.check_call([build, config_path] + input_files_paths)
    except subprocess.CalledProcessError:
        pytest.fail(f"Running gorshochek with config \"{config_path}\"\n" +
                    f"on files [{', '.join(input_files_paths)}] ended with non-zero code")
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
            try:
                subprocess.check_output([clang_path, actual_transform_path, "-o",
                                         path.join(actual_path, f"{i}.o")],
                                        universal_newlines=True,
                                        stderr=subprocess.STDOUT)
            except subprocess.CalledProcessError as e:
                with open(logs_file, "a+") as logs:
                    output = "\n\t".join(e.output.split("\n")[:-1])
                    logs.write(f"- Test: {test_name}\n" +
                               f"  File: {file_name}\n" +
                               f"  Transformation: transformation_{i}.cpp:\n\t" + output + "\n")
                pytest.fail(f"During test {test_name} compilation of {test_name}/transformation_{i}.cpp\n" +
                            "exited with non-zero code, for more details go to tests/clang_log.txt")
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


def test_reorder_function_decls() -> None:
    config_path = path.join(configs_dir_path, "test_reorder_function_decls_config.yaml")
    files = ["test_reorder_function_decls.cpp"]
    _test(files, config_path)


def test_if_else_swap() -> None:
    config_path = path.join(configs_dir_path, "test_if_else_swap_config.yaml")
    files = ["test_if_else_swap.cpp"]
    _test(files, config_path)


def test_for_to_while() -> None:
    config_path = path.join(configs_dir_path, "test_for_to_while_config.yaml")
    files = ["test_for_to_while.cpp"]
    _test(files, config_path)
