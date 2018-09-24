#!/usr/bin/env bash

BINUTILS_VERSION="2.31.1"
GCC_VERSION="8.2.0"
GDB_VERSION="8.2"

export PREFIX="/usr/local/i386elfgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p /tmp/src

install_pacman_pkgs() {
    sudo pacman -S gmp mpfr libmpc bochs nasm
}

# Build and install binutils
install_binutils() {
    cd /tmp/src
    curl -O http://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.gz
    tar -xf binutils-${BINUTILS_VERSION}.tar.gz
    mkdir binutils-build
    cd binutils-build
    ../binutils-${BINUTILS_VERSION}/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
    make all 2>&1 | tee make.log
    sudo make install
}

# Build and install gcc
install_gcc() {
    cd /tmp/src
    curl -O https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz
    tar -xf gcc-${GCC_VERSION}.tar.gz
    mkdir gcc-build
    cd gcc-build
    ../gcc-${GCC_VERSION}/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers
    make all-gcc
    make all-target-libgcc
    sudo make install-gcc
    sudo make install-target-libgcc
}

# Build and install gdb
install_gdb() {
    cd /tmp/src
    curl -O http://ftp.rediris.es/mirror/GNU/gdb/gdb-${GDB_VERSION}.tar.gz
    tar -xf gdb-${GDB_VERSION}.tar.gz
    mkdir gdb-build
    cd gdb-build
    ../gdb-${GDB_VERSION}/configure --target="$TARGET" --prefix="$PREFIX" --program-prefix=i386-elf-
    make
    sudo make install
}

if [ -z "$1" ]; then
    install_binutils
    install_gcc
    install_gdb
    echo "Please add /usr/local/i386elfgcc/bin to your path"
fi

case $1 in
    binutils) install_binutils ;;
    gcc) install_gcc ;;
    gdb) install_gdb ;;
esac
