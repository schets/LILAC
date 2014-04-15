#makefile for the RHS class and derived variants

include ../make.inc

LIB_PATH= $(LIB_MAIN)

SFMT_OBJ =  $(LIB_PATH)/sfmt.o

all: $(SFMT_OBJ)

$(LIB_PATH)/sfmt.o: SFMT.c
	@rm -f $(LIB_PATH)/sfmt.o
	$(CPP) -c SFMT.c $(INC_PATH) -o $(LIB_PATH)/sfmt.o -O3 -m64 -march=native $(FASTMATH) -DHAVE_SSE2 -DSFMT_MEXP=607 -msse2

.PHONY: clean

clean:
	rm -f $(SFMT_OBJ)
