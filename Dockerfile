FROM ubuntu:18.04

RUN apt-get update \
	&& DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends software-properties-common \
	&& add-apt-repository ppa:ubuntu-toolchain-r/test \
	&& DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
                       build-essential \
                       gcc-9 \
                       g++-9 \
                       gcc-9-multilib \
                       g++-9-multilib \
                       git \
                       wget \
                       lsb-release \
                       libomp5 libomp-dev \
                       libtinfo-dev lib32z1-dev \
                       python3-dev python3-pip python3-setuptools

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 10 \
	&& update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 20 \
	&& update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 10 \
	&& update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 20

RUN ln -sf $(which python3) /usr/bin/python \
    && ln -sf $(which pip3) /usr/bin/pip

RUN git clone https://github.com/JetBrains-Research/gorshochek.git
WORKDIR /gorshochek

RUN python -m pip install --upgrade pip
RUN pip install -r requirements.txt
RUN pip install cmake

RUN sh scripts/install_clang.sh

WORKDIR /gorshochek/build
RUN cmake -DLLVM_DIR=build/clang+llvm-9/lib/cmake/llvm \
          -DClang_DIR=build/clang+llvm-9/lib/cmake/clang \
          -DCMAKE_C_COMPILER="/gorshochek/build/clang+llvm-9/bin/clang" \
          -DCMAKE_CXX_COMPILER="/gorshochek/build/clang+llvm-9/bin/clang++" .. \
    && cmake --build .

WORKDIR /gorshochek
RUN pytest . -vv