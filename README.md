# gorshochek
[WIP] A tool for C++ code modification to augment data for clone detection tools

-------------
### Install requirements and build
To install `clang+llvm-9` to use only inside `gorshochek` following commands should be executed: 
```
git clone https://github.com/JetBrains-Research/gorshochek.git
cd gorshochek

# Install clang+llvm-9
sh scripts/install_requirements.sh

# Build
cd build
cmake -DLLVM_DIR=build/clang+llvm-9/lib/cmake/llvm \
      -DClang_DIR=build/clang+llvm-9/lib/cmake/clang \
      -DCMAKE_CXX_COMPILER=/usr/bin/c++ ..
cmake --build .
```