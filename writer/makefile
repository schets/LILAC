#makefile for the controller class and derived variants

include ../make.inc

LIB_PATH= $(LIB_MAIN)

WRITE_OBJ = $(LIB_PATH)/writer.o 

all: $(WRITE_OBJ)


$(LIB_PATH)/writer.o: writer.cpp writer.h data.hpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/writer.o
	$(CPP) -c writer.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/writer.o

.PHONY: clean

clean:
	rm -f $(WRITE_OBJ)
