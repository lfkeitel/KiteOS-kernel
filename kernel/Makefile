DEFAULT_HOST!=../scripts/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../scripts/target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)

C_SOURCES = $(wildcard kernel/*.c memory/*.c $(ARCHDIR)/drivers/*.c $(ARCHDIR)/cpu/*.c)
HEADERS = $(wildcard include/kernel/*.h)
OBJ = ${C_SOURCES:.c=.o $(ARCHDIR)/cpu/interrupt.o}

LINK_LIST = \
$(LDFLAGS) \
$(LIBS) \

kernel.bin: $(ARCHDIR)/boot/kernel_entry.o $(OBJ)
	i386-elf-ld -o $@ -Ttext 0x1000 $^ $(LINK_LIST) --oformat binary

run: os-image.bin
	echo "c" | bochs -q

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: kernel.bin $(ARCHDIR)/boot/bootsect.bin
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $^ $(DESTDIR)$(BOOTDIR)

clean:
	rm -rf *.bin arch/*/boot/*.bin
	rm -rf *.o */*.o */*/*.o */*/*/*.o
