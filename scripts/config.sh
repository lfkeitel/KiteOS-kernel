SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel bootloader"

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./scripts/default-host.sh)}

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-O2 -g'
export CPPFLAGS=''

libgcc="$(dirname -- "$(i686-elf-gcc -print-libgcc-file-name)")"
# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"
export LDFLAGS="--sysroot=$SYSROOT -L$SYSROOT$LIBDIR -L$libgcc"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
    export CC="$CC -isystem=$INCLUDEDIR"
fi
