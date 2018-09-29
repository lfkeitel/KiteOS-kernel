DEFAULT_HOST!=../../scripts/default-host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../../scripts/target-triplet-to-arch.sh $(HOST)

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

ARCHDIR=arch/$(HOSTARCH)

all: $(ARCHDIR)/bootsect.bin

%.bin: %.asm
	nasm $< -f bin -o $@

install: install-bootloader

install-bootloader: $(ARCHDIR)/bootsect.bin
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $< $(DESTDIR)$(BOOTDIR)/stage1.bin

clean:
	rm -rf arch/*/*.bin
