#!/bin/bash

./configure --enable-diskless --with-offset-bits=40 --with-oid-bits=40  --prefix=/opt/fastdb64

make -j

make install
