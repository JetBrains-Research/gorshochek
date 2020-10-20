# gorshochek
[WIP] A tool for C++ code modification to augment data for clone detection tools

-------------
## Usage
1. Clone or download repo from GitHub
```(bash)
git clone https://github.com/JetBrains-Research/gorshochek.git
cd gorshochek
```
2. Install clang with the desired version to use only inside `gorshochek`
```(bash)
./scripts/install_clang.sh
```
3. Build the project from source

* Install CMake
We tested our tool on `macOS`, on `ubuntu-16.04` and `ubuntu-18.04` using `cmake-3.15.2`. To install
`cmake-3.15.2` run the following commands:
```(bash)
wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz
tar -zxvf cmake-3.15.2.tar.gz
cd cmake-3.15.2
./bootstrap
make
sudo make install
# Check the version
cmake --version
```
* Build tool
```(bash)
cd build
cmake -DLLVM_DIR=build/clang+llvm-9/lib/cmake/llvm \
      -DClang_DIR=build/clang+llvm-9/lib/cmake/clang \
      -DCMAKE_C_COMPILER=path-to-gorshochek/build/clang+llvm-9/bin/clang
      -DCMAKE_CXX_COMPILER=path-to-gorshochek/build/clang+llvm-9/bin/clang++ ..
# It is essential to specify the full path to gorshochek
cmake --build .
```
> If you encounter an error on `ubuntu`, try running the following commands:
> ```(bash)
> sudo apt install libtinfo-dev lib32z1-dev build-essential
> ```
4. Run the tool with the needed configuration on C++ files
```(bash)
# You can specify as many files as you need
./build/gorshochek config.yaml file1.cpp file2.cpp
```
Config should be `.yaml` file having the following structure:
```
output path: "path-to-store-transformations"
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
More examples can be found in `tests` folder

--------------
## Transformations

- [x] Identity transformation
- [x] Add, remove comments
- [ ] Useless variables, functions, defines
- [x] Rename variables, functions
- [ ] Random change between `x++`, `++x`, `x+=1`, `x=x+1`
- [ ] Change the signature of functions by making all the variables global
- [x] Rearranging function declarations
- [x] Replace `for` with `while` and vice versa 
- [ ] Replace `for` loop iteration using indexing with `for` loop with `auto`
- [ ] Change types from `int` to `long int` and so on
- [ ] Replace `printf` with `std::cout`
- [x] Swap `if` and `else` blocks and change the corresponding condition inside `if`
- [ ] Add wrappers for functions

--------------
## Documentations

More detailed documentation can be found in [DOCS.md](DOCS.md)

---------------
## Contribution

If want to contribute to the project and add new transformation (e.g. `Example`, note that `Example` 
is just a name of a transformation) the following classes should be implemented:
- `ExampleTransformation` derived from `ITransformation` --  class that aggregates all the sufficient
information from `config.yaml` and creates and instances of `ExampleASTConsumer` using 
`getConsumer` method
- `ExampleASTConsumer` derived from `ASTConsumer`
- `ExampleASTVisitor` derived from `RecursiveASTVisitor<ExampleASTVisitor>`
