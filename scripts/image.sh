#!/bin/sh
set -e
. ./scripts/build.sh

mkdir -p build

cat sysroot/boot/stage1.bin sysroot/boot/kernel.bin > build/os-image.bin
