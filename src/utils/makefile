#makefile for utility functions
include ../make.inc
LIB_PATH= $(LIB_MAIN)

UTIL_OBJ=$(LIB_PATH)/utils.o $(LIB_PATH)/main.o  $(LIB_PATH)/mempool.o

all: $(UTIL_OBJ)

$(LIB_PATH)/utils.o: utils.cpp $(INLINE HEADERS)
	@rm -f $(LIB_PATH)/utils.o
	$(CPP) -c utils.cpp $(CFLAGS) $(INC_PATH) -o $(LIB_PATH)/utils.o

$(LIB_PATH)/main.o: main.cpp $(INLINE HEADERS)
	@rm -f $(LIB_PATH)/main.o
	$(CPP) -c main.cpp $(CFLAGS) $(INC_PATH) -o $(LIB_PATH)/main.o

$(LIB_PATH)/mempool.o: mempool.cpp $(INLINE HEADERS)
	@rm -f $(LIB_PATH)/mempool.o
	$(CPP) -c mempool.cpp $(CFLAGS) $(INC_PATH) -o $(LIB_PATH)/mempool.o


.PHONY: clean
clean:
	rm -f $(UTIL_OBJ)
