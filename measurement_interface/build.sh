#/bin/bash

mkdir -p ../build
g++ main.cpp ../copositive_checker.cpp ../clique_number/clique_finder.cpp -o ../build/measurement_interface
