SUBDIRS := $(wildcard */.)

export ENE:=../../bin/ene
export ENEFLAGS:=-b 32 -t ../../src/templates -a gas

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
