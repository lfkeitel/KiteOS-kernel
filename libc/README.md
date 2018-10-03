# KiteOS libc

This is an implemented of libc for KiteOS. It can build two libraries. `libc` is
the standard C library. This is the library used by user programs. `libk` is a
version built specifically for the kernel. It directly invokes kernel functions
that are unavailable in user programs.
