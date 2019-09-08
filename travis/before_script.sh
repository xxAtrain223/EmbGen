#!/usr/bin/env bash
set -e
mkdir build
cd build

cmake -DEmbGen_ENABLE_TESTING:BOOL=ON ..