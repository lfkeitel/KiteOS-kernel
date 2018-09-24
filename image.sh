#!/bin/sh
set -e
. ./build.sh

mkdir -p boot

cat sysroot/boot/bootsect.bin sysroot/boot/kernel.bin > boot/os-image.bin
