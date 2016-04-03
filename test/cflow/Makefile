SOURCES:=$(wildcard *.ene)
OUTS:=$(patsubst %.ene, %.out, $(SOURCES))

all: $(OUTS)

%.out: %.ene
	$(ENE) -i $< -o $@ $(ENEFLAGS)
