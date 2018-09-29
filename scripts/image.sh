#!/bin/sh
set -e
. ./scripts/build.sh

mkdir -p boot

cat sysroot/boot/bootsect.bin sysroot/boot/kernel.bin > boot/os-image.bin
