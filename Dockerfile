FROM ubuntu:18.04

LABEL desc="Dockerfile for gorshochek -- tool for c++ source code augmetration"

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
                       fdupes \
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

WORKDIR /gorshochek
COPY . /gorshochek

RUN python -m pip install --upgrade pip
RUN pip install -r requirements.txt
RUN pip install cmake==3.18.4

RUN sh scripts/install_clang.sh

RUN cd ./build \
    && cmake -DLLVM_DIR=build/clang+llvm-9/lib/cmake/llvm \
             -DClang_DIR=build/clang+llvm-9/lib/cmake/clang \
             -DCMAKE_C_COMPILER="/gorshochek/build/clang+llvm-9/bin/clang" \
             -DCMAKE_CXX_COMPILER="/gorshochek/build/clang+llvm-9/bin/clang++" .. \
    && cmake --build .

RUN mkdir data && mkdir data/input && mkdir data/output

ENTRYPOINT ["./build/gorshochek", "./data/input", "./data/output"]
CMD ["./config.yaml"]