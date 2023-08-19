#!/bin/bash

pushd "src"
wget -q "https://github.com/zpl-c/librg/releases/download/v7.2.1/librg.h" -O librg.h
popd

mkdir build -p
pushd "build"
cmake ..
make
chmod +x example
popd
