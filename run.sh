#!/bin/sh
set -e
. ./image.sh

echo "c" | bochs -q
