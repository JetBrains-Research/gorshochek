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
      -DCMAKE_CXX_COMPILER=/usr/bin/c++ ..
cmake --build .
```
4. Run the tool with the needed configuration on C++ files
```(bash)
# TODO
```