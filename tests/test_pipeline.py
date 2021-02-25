import inspect
import shutil
import subprocess
from os import path, listdir

import pytest

configs_dir_path = path.join("tests", "configs")
logs_file = path.join("tests", "clang_logs.txt")
default_expected_path = path.join("tests", "resources", "expected")
actual_path = path.join("tests", "resources", "actual")
input_path = path.join("tests", "resources", "input")
clang_path = path.join("c++")
tricky_path = path.join("scripts", "..", "tests", "..", "tests", "resources", "input")
build = path.join("build", "gorshochek")


def _parse_logs(text):
    logs_ = text.split(" - ")[1:]
    logs_ = {
        elem.rsplit("\n")[0]: set(elem.split("\n")[1:]) for elem in logs_
    }
    return logs_


def _test(
        input_: str,
        config_path: str,
        expected_path: str = default_expected_path,
        check_compilation: bool = True
) -> None:
    # Getting the caller function name
    test_name = inspect.stack()[1].function
    full_input_path = path.join(input_path, input_)
    files = listdir(full_input_path) if path.isdir(full_input_path) else [input_]
    try:
        subprocess.check_call([build, full_input_path, actual_path, config_path])
    except subprocess.CalledProcessError:
        pytest.fail(f"Running gorshochek with config \"{config_path}\"\n" +
                    f"on files [{full_input_path}] ended with non-zero code")
    assert path.exists(actual_path), f"Transformed files folder \"{actual_path}\" does not exists"

    for file in files:
        file_name, _ = file.rsplit(".")
        expected_file_dir = path.join(expected_path, file_name)
        actual_file_dir = path.join(actual_path, file_name)
        expected_num_transforms = len(listdir(expected_file_dir))
        actual_num_transforms = len(listdir(actual_file_dir))
        assert actual_num_transforms == expected_num_transforms, \
            f"Num transformed files is not equal to num expected"

        expected_descr_path = path.join(expected_file_dir, f"description.txt")
        actual_descr_path = path.join(actual_file_dir, f"description.txt")
        with open(expected_descr_path, "r") as expected:
            expected_desrc = expected.read()
        with open(actual_descr_path, "r") as transformed:
            transformed_desrc = transformed.read()
        assert expected_desrc == transformed_desrc, "Actual and expected description mismatch"

        for i in range(expected_num_transforms - 1):
            expected_transform_path = path.join(expected_file_dir, f"transformation_{i}.cpp")
            actual_transform_path = path.join(actual_file_dir, f"transformation_{i}.cpp")
            if check_compilation:
                try:
                    subprocess.check_output([clang_path, "-std=c++11", actual_transform_path, "-o",
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
            assert transformed_data == expected_data, "Actual and expected files mismatch"
    expected_log_path = path.join(expected_path, "log_1.txt")
    actual_log_path = path.join(actual_path, "log_1.txt")
    if path.exists(actual_log_path) and path.exists(expected_log_path):
        with open(expected_log_path) as exp:
            expected_log = _parse_logs(exp.read())
        with open(actual_log_path) as act:
            actual_log = _parse_logs(act.read())
        assert expected_log == actual_log
    shutil.rmtree(actual_path)


def test_pipeline() -> None:
    config_path = path.join(configs_dir_path, "test_pipeline_config.yaml")
    input_ = "test_pipeline"
    _test(input_, config_path, expected_path=path.join(default_expected_path, input_))


def test_remove_comments() -> None:
    config_path = path.join(configs_dir_path, "test_remove_comments_config.yaml")
    input_ = "test_remove_comments.cpp"
    _test(input_, config_path)


def test_add_comments() -> None:
    config_path = path.join(configs_dir_path, "test_add_comments_config.yaml")
    input_ = "test_add_comments.cpp"
    _test(input_, config_path)


def test_rename_entities() -> None:
    config_path = path.join(configs_dir_path, "test_rename_entities_config.yaml")
    input_ = "test_rename_entities.cpp"
    _test(input_, config_path)


def test_reorder_function_decls() -> None:
    config_path = path.join(configs_dir_path, "test_reorder_function_decls_config.yaml")
    input_ = "test_reorder_function_decls.cpp"
    _test(input_, config_path)


def test_if_else_swap() -> None:
    config_path = path.join(configs_dir_path, "test_if_else_swap_config.yaml")
    input_ = "test_if_else_swap.cpp"
    _test(input_, config_path)


def test_printf_to_cout() -> None:
    config_path = path.join(configs_dir_path, "test_printf_to_cout_config.yaml")
    input_ = "test_printf_to_cout.cpp"
    _test(input_, config_path)


def test_for_to_while() -> None:
    config_path = path.join(configs_dir_path, "test_for_to_while_config.yaml")
    input_ = "test_for_to_while.cpp"
    _test(input_, config_path)


def test_while_to_for() -> None:
    config_path = path.join(configs_dir_path, "test_while_to_for_config.yaml")
    input_ = "test_while_to_for.cpp"
    _test(input_, config_path)


def test_open_macros() -> None:
    config_path = path.join(configs_dir_path, "test_open_macros_config.yaml")
    input_ = "test_open_macros.cpp"
    _test(input_, config_path)


def test_logging() -> None:
    config_path = path.join(configs_dir_path, "test_logging_config.yaml")
    input_ = "test_logging"
    _test(input_, config_path, expected_path=path.join(default_expected_path, input_), check_compilation=False)
