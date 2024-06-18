#!/bin/bash
cd ./Toolchain/
make clean all
cd ..
if [[ "$1" == "-b" ]]; then
  make clean buddy
else
  make clean all
fi
