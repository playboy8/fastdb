#!/bin/bash

./configure --enable-diskless --with-offset-bits=38 CXXFLAGS=-ggdb3  --prefix=/opt/fastdb64 
 
#--with-oid-bits=32 --enable-debug --with-debug-level=all  --enable-debug
make -j

make install


