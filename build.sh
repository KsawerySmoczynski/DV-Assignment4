#!/bin/sh

THREADS=$(nproc)

BASE_DIR='assignment 4'
CMAKE_DIR="$BASE_DIR/make/cmake"
BUILD_DIR="$BASE_DIR/bin"

cmake "$CMAKE_DIR" -B "$BUILD_DIR"

make -j "$THREADS" -C "$BUILD_DIR"

cp "$BUILD_DIR/assignment4" "$BASE_DIR"
