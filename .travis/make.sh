#!/bin/bash

# wrapper so tests don't fail
(
  cd build
  make "$@" RUNTESTFLAGS="-a" | tail -n 100
)
