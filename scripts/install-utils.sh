#!/usr/bin/env bash
set -e

BINUTILS_VERSION="2.31.1"
GCC_VERSION="8.2.0"
GDB_VERSION="8.2"

export PREFIX="/usr/local/i686elfgcc"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

CPUNUM=$(cat /proc/cpuinfo | awk '/^processor/{print $3}' | wc -l)

mkdir -p /tmp/src

install_pacman_pkgs() {
    sudo pacman -S gmp mpfr libmpc bochs nasm make gcc
}

# Build and install binutils
build_binutils() {
    cd /tmp/src
    curl -O http://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.gz
    tar -xf binutils-${BINUTILS_VERSION}.tar.gz
    rm -rf binutils-build
    mkdir binutils-build
    cd binutils-build
    ../binutils-${BINUTILS_VERSION}/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
    make -j ${CPUNUM} all 2>&1 | tee make.log
    install_binutils
}

install_binutils() {
    cd /tmp/src/binutils-build
    sudo make install
}

# Build and install gcc
build_gcc() {
    cd /tmp/src
    curl -O https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz
    tar -xf gcc-${GCC_VERSION}.tar.gz
    rm -rf gcc-build
    mkdir gcc-build
    cd gcc-build
    ../gcc-${GCC_VERSION}/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c,c++ --without-headers
    make -j ${CPUNUM} all-gcc
    make -j ${CPUNUM} all-target-libgcc
    install_gcc
}

install_gcc() {
    cd /tmp/src/gcc-build
    sudo make install-gcc
    sudo make install-target-libgcc
}

# Build and install gdb
build_gdb() {
    cd /tmp/src
    curl -O http://ftp.rediris.es/mirror/GNU/gdb/gdb-${GDB_VERSION}.tar.gz
    tar -xf gdb-${GDB_VERSION}.tar.gz
    rm -rf gdb-build
    mkdir gdb-build
    cd gdb-build
    ../gdb-${GDB_VERSION}/configure --target="$TARGET" --prefix="$PREFIX" --program-prefix=i686-elf-
    make -j ${CPUNUM}
    install_gdb
}

install_gdb() {
    cd /tmp/src/gdb-build
    sudo make install
}

if [ -z "$1" ]; then
    build_binutils
    build_gcc
    # build_gdb
    echo "Please add /usr/local/i686elfgcc/bin to your path"
fi

case $1 in
    pacman) install_pacman_pkgs ;;
    binutils) build_binutils ;;
    gcc) build_gcc ;;
    gdb) build_gdb ;;
    install)
        install_binutils
        install_gcc
        ;;
esac
