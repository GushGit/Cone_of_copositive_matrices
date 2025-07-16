#/bin/bash

mkdir -p ../build
g++ main.cpp ../copositive_checker.cpp -o ../build/copositive_test
time ./../build/copositive_test
