#!/bin/sh
set -e
. ./scripts/image.sh

echo "c" | bochs -q
