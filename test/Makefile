SUBDIRS := $(wildcard */.)

export ENE:=../../bin/ene
export ENEFLAGS:=-b 32 -a nasm -t ../../src/templates

all: $(SUBDIRS)

test: 

clean:
	@find . -name '*.o' -delete
	@find . -name '*.out' -delete
	@find . -name '*.obj' -delete
	@find . -name '*.nasm' -delete
	@find . -name '*.s' -delete

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
