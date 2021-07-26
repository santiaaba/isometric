#!/bin/bash

cd libs
rm *.o
gcc -c point.c
gcc -c vector.c
gcc -c isometric.c
gcc -c engine.c
gcc -c rect.c
cd ..
gcc demo1.c libs/rect.o libs/vector.o libs/engine.o libs/isometric.o libs/point.o -lm -lSDL2_image -lSDL2main -lSDL2 -o demo1

