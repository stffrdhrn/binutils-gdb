#!/bin/bash

(
  mkdir build
  cd build
  ../configure --target=or1k-elf --prefix=/tmp
  make -j2
  make install
)
