DEFAULT_HOST!=../scripts/default-host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../scripts/target-triplet-to-arch.sh $(HOST)

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

ARCHDIR=arch/$(HOSTARCH)

STAGES=stage2 stage1
STAGESCLEAN=$(addsuffix clean, $(STAGES))
STAGESINSTALL=$(addsuffix install, $(STAGES))

.PHONY: $(STAGES) clean install

all: $(STAGES)

$(STAGES):
	$(MAKE) -C $@

install: $(STAGESINSTALL)

%install:
	$(MAKE) -C $* install

clean: $(STAGESCLEAN)

%clean:
	$(MAKE) -C $* clean
