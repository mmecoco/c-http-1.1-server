#!/usr/bin/env powershell

git submodule update --init --recursive
pip install conan

mkdir -Force ~/.conan > $null
cp Windows/remotes.json ~/.conan/remotes.json
mkdir -Force build > $null

cd build
conan install --build=missing ..

cmake -DCMAKE_BUILD_TYPE=Release ..
