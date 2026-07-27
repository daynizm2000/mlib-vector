#!/bin/bash


cd example/


if [ ! -f "example_bin" ]; then
    make
fi


./example_bin

rm example_bin