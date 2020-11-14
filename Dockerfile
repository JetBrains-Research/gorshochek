FROM ubuntu:18.04

RUN apt-get update \
    && apt-get install --no-install-recommends -y \
                       build-essential \
                       git \
                       wget \
                       lsb-release \
                       libtinfo-dev lib32z1-dev \
                       python3-dev python3-pip python3-setuptools

RUN ln -sf $(which python3) /usr/bin/python \
    && ln -sf $(which pip3) /usr/bin/pip

WORKDIR /gorshochek
COPY . /gorshochek

RUN python -m pip install --upgrade pip
RUN pip install -r requirements.txt

RUN sh scripts/install_clang.sh

RUN wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz
RUN tar -zxvf cmake-3.15.2.tar.gz

WORKDIR cmake-3.15.2
RUN ./bootstrap \
    && make \
    && make install

WORKDIR ../build
RUN cmake -DLLVM_DIR=build/clang+llvm-9/lib/cmake/llvm \
          -DClang_DIR=build/clang+llvm-9/lib/cmake/clang \
          -DCMAKE_C_COMPILER="/gorshochek/build/clang+llvm-9/bin/clang" \
          -DCMAKE_CXX_COMPILER="/gorshochek/build/clang+llvm-9/bin/clang++" .. \
    && cmake --build .