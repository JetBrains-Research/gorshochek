# gorshochek
[WIP] A tool for C++ code modification to augment data for clone detection tools

-------------
## Installation
Clone or download repo from GitHub
```(bash)
git clone https://github.com/JetBrains-Research/gorshochek.git
cd gorshochek
```
### Docker
It is more preferable to run `gorshochek` using `Docker`. 

To build docker image run:
```(bash)
sudo docker build -t gorshochek .
```
# Usage
Then to run a container from just created image run:
```(bash)
sh ./scripts/run.sh input_folder output_folder
```
Note that `input_folder` will be traversed recursively and every `.cpp` file from it will be transformed.
To specify which exact transformations to apply edit file `config.yaml`, which should have the following structure:
```
n_transformations: 3
transformations:
  - identity transform:
      p: 0.99
  - add comments:
  ...
```
The output will have structure as follows:
```
output_path
├── log.txt
├── file1
|   ├── description.txt
│   ├── transformation_0.cpp
|   ├── transformation_1.cpp
│   └── transformation_2.cpp
├── file2
|   ├── description.txt
│   ├── transformation_0.cpp
|   ├── transformation_1.cpp
│   └── transformation_2.cpp
...
```
Here `log.txt` file contains data on how many errors appeared after applying each transformation from `config.yaml`.
Logs are stored in the following format:
```
 - transformation_1
file1/transformation_1.cpp	1
file2/transformation_1.cpp	1
file3/transformation_1.cpp	1
 - transformation_2
file1/transformation_1.cpp	1
...
```
The `log.txt` file is split in several blocks, each titled as follows: ` - transformation_i`. 
Each block belongs to a certain transformation. After block title each line corresponds to a 
transformed file and the number of errors found in that file, for instance: `file1/transformation_1.cpp  1`. 
More examples can be found in `tests` folder

--------------
## Transformations

- [x] Identity transformation
- [x] Add, remove comments
- [x] Rename variables, functions
- [x] Swap `if` and `else` blocks and change the corresponding condition inside `if`
- [x] Rearranging function declarations
- [x] Replace `for` with `while` and vice versa 
- [x] Replace `printf` with `std::cout`
- [x] Open macros
- [ ] Random change between `x++`, `++x`, `x+=1`, `x=x+1`
- [ ] Change the signature of functions by making variables global
- [ ] Replace `std::cout` with `printf`
- [ ] Useless variables, functions, defines

--------------
## Documentations

More detailed documentation can be found in [DOCS.md](DOCS.md)

---------------
## Contribution

If want to contribute to the project and add new transformation (e.g. `Example`, note that `Example` 
is just a name of a transformation) the following classes should be implemented:
- `ExampleTransformation` derived from `ITransformation` --  class that aggregates all the sufficient
information from `config.yaml` and creates instances of `ExampleASTConsumer` using 
`getConsumer` method
- `ExampleASTConsumer` derived from `ASTConsumer`
- `ExampleASTVisitor` derived from `RecursiveASTVisitor<ExampleASTVisitor>`
