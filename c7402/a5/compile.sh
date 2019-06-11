#!/bin/sh

mkdir -p bin
mkdir -p build
mkdir -p outputs
rm build/*
cd build
cmake ../
make