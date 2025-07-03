#/bin/bash

mkdir -p build
g++ copositive_test.cpp is_copositive.cpp -o build/copositive_test
./build/copositive_test
