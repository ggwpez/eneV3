SUBDIRS := $(wildcard */.)

ENE:=../../bin/ene
ENEBITS:=32
ENEAS:=nasm
ENEOP:=2
ENEFLAGS:=-b $(ENEBITS) -t ../../src/templates -a $(ENEAS) -O$(ENEOP)

export ENE
export ENEFLAGS

all: $(SUBDIRS)

test:
	sh test.sh

retest: clean all test

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

clean:
	@find . -name '*.o' -delete
	@find . -name '*.out' -delete
	@find . -name '*.obj' -delete
	@find . -name '*.nasm' -delete
	@find . -name '*.gas' -delete
