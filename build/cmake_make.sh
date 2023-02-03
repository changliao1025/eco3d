#!/bin/bash
rm -rf CMakeCache.txt
cmake CMakeLists.txt
make install
