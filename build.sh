#!/bin/bash

mkdir build -p
pushd "build"
cmake .. & make
chmod +x example
popd
