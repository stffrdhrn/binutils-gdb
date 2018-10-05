#!/bin/bash

(
  cd $HOME
  if [ ! -d "or1k-utils" ] ; then
    git clone --depth=5 https://github.com/stffrdhrn/or1k-utils.git
  fi
)
