#!/usr/bin/env bash
set -e

pushd build
make -j2

cd parser
./EmbGenParserTest

popd

cd parser
cppcheck src/*.cpp include/EmbGen/*.hpp