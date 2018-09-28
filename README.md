# KiteOS

KiteOS is a hobby operating system that runs on the x86 architecture. This repository
contains a custom bootloader as KiteOS does not currently support being booted
from a multiboot program such as GRUB or LILO.

## Building

To build KiteOS, you will need a copy of GCC build to cross compile to i386-elf.
You will also need `gmp`, `mpfr`, `libmpc`, and `nasm`. You will also want `bochs` to
run the built kernel and bootloader.

You can install the cross-compiled toolchain with the `install-utils.sh` script.
It will build the needed tools in `/tmp` and install them to `/usr/local/i386elfgcc`.
If you want to use a different directory, you can change the PREFIX variable in
the script.

Run the `image.sh` script to build the bootloader, kernel, and libc binaries.
The bootable image will be in `boot/os-image.bin`.

You can run the OS with the `run.sh` script. It will load Bochs with a known
working configuration. If everything worked successfully, you should see the
banner "Welcome to KiteOS!" and a shell prompt.

## License

The KiteOS kernel, bootloader, and libc source are all licensed under the
truly free New BSD License (BSD-3-Clause). The full license text is available
in the [LICENSE](LICENSE) file.
