#makefile for utility functions
include ../make.inc
LIB_PATH= $(LIB_MAIN)

TYPE_OBJ=$(LIB_PATH)/item_factory.o

all: $(TYPE_OBJ)

$(LIB_PATH)/item_factory.o: item_factory.cpp $(INLINE HEADERS)
	@rm -f $(LIB_PATH)/item_factory.o
	$(CPP) -c item_factory.cpp $(CFLAGS) $(INC_PATH) -o $(LIB_PATH)/item_factory.o


.PHONY: clean
clean:
	rm -f $(TYPE_OBJ)
