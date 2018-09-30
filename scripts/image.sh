#!/bin/sh
set -e
. ./scripts/build.sh

mkdir -p build

FILES=(
    sysroot/boot/stage1.bin
    sysroot/boot/stage2.bin
    sysroot/boot/kernel.bin
)

cat ${FILES[@]} > build/os-image.bin
