#!/bin/bash

# a little script to rebuild the package in your working directory

rm -f cscope.*
NOCONFIGURE=1 ./autogen.sh
./configure.sh
make clean
make cscope
cscope -b
make -j$(($(nproc 2>/dev/null||echo 4)<<1)) clean all README
