DEFAULT_HOST!=../scripts/default-host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../scripts/target-triplet-to-arch.sh $(HOST)

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

ARCHDIR=arch/$(HOSTARCH)

STAGES=stage1# stage2

.PHONY: $(STAGES)

all: $(STAGES)

$(STAGES):
	$(MAKE) -C $@

install: install-$(STAGES)

install-$(STAGES): $(STAGES)
	$(MAKE) -C $< install

clean: clean-$(STAGES)

clean-$(STAGES): $(STAGES)
	$(MAKE) -C $< clean
