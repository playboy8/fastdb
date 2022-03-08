#!/bin/bahs

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/fastdb64/lib
rm -rf  build
mkdir -p build
cd build
cmake ..
make