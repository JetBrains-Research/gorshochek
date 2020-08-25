from os import path, listdir
import subprocess
import shutil

config_path = path.join("tests", "test_pipeline_config.yaml")
expected_path = path.join("tests", "resources", "expected")
actual_path = path.join("tests", "resources", "actual")
input_path = path.join("tests", "resources", "input")
build = path.join("build", "gorshochek")


def test_pipeline() -> None:
    input_files_paths = [
        path.join("tests", "resources", "input", file) for file in listdir(input_path)
    ]
    subprocess.check_call([build, config_path] + input_files_paths)
    assert path.exists(actual_path), f"Transformed files folder \"{actual_path}\" does not exists"

    for file in listdir(input_path):
        file_name, _ = file.rsplit(".")
        expected_file_dir = path.join(expected_path, file_name)
        actual_file_dir = path.join(actual_path, file_name)
        expected_num_transforms = len(listdir(expected_file_dir))
        actual_num_transforms = len(listdir(actual_file_dir))
        assert actual_num_transforms == expected_num_transforms, \
            f"Num transformed files is not equal to num expected"
        for i in range(expected_num_transforms):
            expected_transform_path = path.join(expected_file_dir, f"transformation_{i}.cpp")
            actual_transform_path = path.join(actual_file_dir, f"transformation_{i}.cpp")
            with open(expected_transform_path, "r") as expected:
                expected_data = expected.read()
            with open(actual_transform_path, "r") as transformed:
                transformed_data = transformed.read()
            assert transformed_data == expected_data, "Actual and expected files mismatch "
    shutil.rmtree(actual_path)
