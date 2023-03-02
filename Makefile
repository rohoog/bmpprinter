
CFLAGS=-O -g -Wall

bmp:bmp.c

# compile-and-run
run%: %
	./$< $F

.PHONY: run%
