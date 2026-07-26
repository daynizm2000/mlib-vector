#!/bin/bash


if [ ! -f vector ]; then
    make
fi


./vector

rm vector