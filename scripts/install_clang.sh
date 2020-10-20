#!/bin/bash
# Run script from gorshochek dir using command:
#    sh scripts/install_clang.sh

BUILD_DIR=./build

OS=$(uname -s)
case "$OS" in
  Darwin*)
    echo "OSX"
    DISTR_VERSION="darwin-apple"
    ;;
  Linux*)
    echo "LINUX"
    DISTR=$(lsb_release -is | tr "[:upper:]" "[:lower:]")
    VERSION=$(lsb_release -sr | tr "[:upper:]" "[:lower:]")
    DISTR_VERSION="linux-gnu-${DISTR}-${VERSION}"
    ;;
  *)
    echo "unknown: $OS"
    ;;
esac

if [ ! -d "$BUILD_DIR" ]
then
  mkdir "$BUILD_DIR"
fi

DISTR_ARCHIVE="clang+llvm-9.0.0-x86_64-${DISTR_VERSION}.tar.xz"
DISTR_LINK="https://releases.llvm.org/9.0.0/${DISTR_ARCHIVE}"
if [ ! -f "$BUILD_DIR"/"$DISTR_ARCHIVE" ]
then
  wget "$DISTR_LINK" -P "$BUILD_DIR"
fi

tar -xvf "$BUILD_DIR"/"$DISTR_ARCHIVE" -C "$BUILD_DIR"
mv "$BUILD_DIR"/"clang+llvm-9.0.0-x86_64-${DISTR_VERSION}" "$BUILD_DIR"/"clang+llvm-9"
ln -s "$BUILD_DIR"/"clang+llvm-9/lib" "lib"

