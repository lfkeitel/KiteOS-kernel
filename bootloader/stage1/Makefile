DEFAULT_HOST!=../../scripts/default-host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../../scripts/target-triplet-to-arch.sh $(HOST)

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

ARCHDIR=arch/$(HOSTARCH)

all: $(ARCHDIR)/stage1.bin

%.bin: %.asm
	set -e ;\
	SECTORS=$$(($$(stat -c %s ../stage2/$(ARCHDIR)/stage2.bin)/512)) ;\
	nasm $< -f bin -o $@ -dstage2_sect_count=$$SECTORS

install: install-bootloader

install-bootloader: $(ARCHDIR)/stage1.bin
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $< $(DESTDIR)$(BOOTDIR)

clean:
	find . -name '*.bin' -delete
