#!/usr/bin/env bash
set -e

pushd build
make -j2

./EmbGenTest

popd

cppcheck src/*.cpp include/EmbGen/*.hpp
