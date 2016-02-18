#!/bin/sh
cd tcusrs
make
gcc -Wall -o tcusrs main.o
cd ..