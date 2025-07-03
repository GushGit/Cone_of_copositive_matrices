#/bin/bash

mkdir -p build
g++ clique_finder.cpp is_copositive.cpp -o build/clique_finder
./build/clique_finder
