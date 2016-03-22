SUBDIRS := $(wildcard */.)

export ENE:=../../bin/ene
export ENEFLAGS:=-b 32 -a nasm -t ../../src/templates

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
