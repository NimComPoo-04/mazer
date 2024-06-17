#!/bin/sh
export LD_LIBRARY_PATH=~/opt/raylib-5.0_linux_amd64/lib/
gcc *.c -Wall -ggdb -O0 -o mazer -L ~/opt/raylib-5.0_linux_amd64/lib/  -I ~/opt/raylib-5.0_linux_amd64/include/ -lraylib -lm
./mazer
