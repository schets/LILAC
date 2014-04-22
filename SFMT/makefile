#makefile for the RHS class and derived variants

include ../make.inc

LIB_PATH= $(LIB_MAIN)

DSFMT_OBJ =  $(LIB_PATH)/dsfmt.o $(LIB_PATH)/sfmt.o


all: $(DSFMT_OBJ)

$(LIB_PATH)/dsfmt.o: dSFMT.c
	@rm -f $(LIB_PATH)/dsfmt.o
	$(CPP) -c dSFMT.c $(INC_PATH) -o $(LIB_PATH)/dsfmt.o -O3 -m64 -march=native $(FASTMATH) -DHAVE_SSE2 -DDSFMT_MEXP=521
$(LIB_PATH)/sfmt.o: SFMT.c
	@rm -f $(LIB_PATH)/sfmt.o
	$(CPP) -c SFMT.c $(INC_PATH) -o $(LIB_PATH)/sfmt.o -O3 -m64 -march=native $(FASTMATH) -DHAVE_SSE2 -DSFMT_MEXP=607

.PHONY: clean

clean:
	rm -f $(DSFMT_OBJ)
