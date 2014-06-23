#!/bin/bash
git submodule init
git submodule update
rm -rf src/ffts
rm -rf src/eigen3
mv eigen3 src/
mv ffts src/

