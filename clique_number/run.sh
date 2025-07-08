#/bin/bash

mkdir -p ../build
g++ main.cpp clique_finder.cpp ../copositive_checker.cpp -o ../build/clique_finder
time ./../build/clique_finder
