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
```(bash)
cd build
cmake -DLLVM_DIR=build/clang+llvm-9/lib/cmake/llvm \
      -DClang_DIR=build/clang+llvm-9/lib/cmake/clang \
      -DCMAKE_CXX_COMPILER=/usr/bin/c++ 
      -DCMAKE_C_COMPILER=path-to-gorshochek/build/clang+llvm-9/bin/clang
      -DCMAKE_CXX_COMPILER=path-to-gorshochek/build/clang+llvm-9/bin/clang++ ..
# It is essential to specify the full path to gorshochek
cmake --build .
```
4. Run the tool with the needed configuration on C++ files
```(bash)
# You can specify as many files as you need
./build/gorshochek config file1.cpp file2.cpp
```
Config should be `.yaml` file having the following structure:
```
output path: "path-to-store-transformations"
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

--------------
## Transformations

- [x] Identity transformation
- [x] Add, remove comments
- [ ] Useless variables, functions, defines
- [ ] Rename variables, functions
- [ ] Random change between `x++`, `++x`, `x+=1`, `x=x+1`
- [ ] Change the signature of functions by making all the variables global
- [ ] Rearranging function declarations
- [ ] Replace `for` with `while` and vice versa 
- [ ] Replace `for` loop iteration using indexing with `for` loop with `auto`
- [ ] Change types from `int` to `long int` and so on
- [ ] Replace `printf` with `std::cout`
- [ ] Swap `if` and `else` blocks and change the corresponding condition inside `if`
- [ ] Add wrappers for functions
