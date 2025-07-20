#/bin/bash

mkdir -p ../build
g++ main.cpp -o ../build/bivalue_checker
time ./../build/bivalue_checker
