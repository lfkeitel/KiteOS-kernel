DEFAULT_HOST!=../../scripts/default-host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../../scripts/target-triplet-to-arch.sh $(HOST)

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

ARCHDIR=arch/$(HOSTARCH)

all: $(ARCHDIR)/stage2.bin

%.bin: %.asm
	nasm $< -f bin -o $@.stub
	cp $@.stub $@
	head -c $$(((512-$$(stat -c %s $@.stub)%512)%512)) /dev/zero >> $@
	rm $@.stub

install: install-bootloader

install-bootloader: $(ARCHDIR)/stage2.bin
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $< $(DESTDIR)$(BOOTDIR)

clean:
	find . -name '*.bin' -delete
