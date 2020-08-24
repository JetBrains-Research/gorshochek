from os import path, listdir
import subprocess
import shutil

config_path = path.join("tests", "test_config.yaml")
files = ["test1.cpp", "test2.cpp", "test3.cpp"]
file_paths = [
    path.join("tests", "resources", file) for file in files
]

build = path.join("build", "gorshochek")

def test_word_count() -> None:
    subprocess.check_call([build, config_path] + file_paths)
    etalon_path = path.join("tests", "etalon")
    transformed_path = path.join("tests", "transformed")
    assert path.exists(transformed_path)

    for file in files:
        file_name, _ = file.rsplit(".")
        etalon_file_dir = path.join(etalon_path, file_name)
        transformed_file_dir = path.join(transformed_path, file_name)
        etalon_num_transforms = len(listdir(etalon_file_dir))
        transformed_num_transforms = len(listdir(transformed_file_dir))
        assert transformed_num_transforms == etalon_num_transforms
        for i in range(etalon_num_transforms):
            etalon_transform_path = path.join(etalon_file_dir, f"transformation_{i}.cpp")
            transformed_transform_path = path.join(transformed_file_dir, f"transformation_{i}.cpp")
            with open(etalon_transform_path, "r") as etalon:
                etalon_data = etalon.read()
            with open(transformed_transform_path, "r") as transformed:
                transformed_data = transformed.read()
            assert transformed_data == etalon_data
    # shutil.rmtree(transformed_path)
