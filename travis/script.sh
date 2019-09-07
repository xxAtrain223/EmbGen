#!/usr/bin/env bash
set -e

pushd build
make -j2

pushd parser
./EmbGenParserTest
popd

pushd gen
./_EmbGenTest
popd

popd

pushd parser
cppcheck src/*.cpp include/Parser/EmbGen/*.hpp
popd

pushd gen
cppcheck src/*.cpp include/EmbGen/*.hpp
popd
