#makefile for utility functions
include ../make.inc
LIB_MAIN = ../lib

LIB_PATH= $(LIB_MAIN)

UTIL_OBJ=$(LIB_PATH)/utils.o $(LIB_PATH)/main.o

all: $(UTIL_OBJ)

$(LIB_PATH)/utils.o: utils.cpp $(INLINE HEADERS)
	@rm -f $(LIB_PATH)/util.o
	$(CPP) -c utils.cpp $(CFLAGS) $(INC_PATH) -o $(LIB_PATH)/utils.o

$(LIB_PATH)/main.o: main.cpp $(INLINE HEADERS)
	@rm -f $(LIB_PATH)/main.o
	$(CPP) -c main.cpp $(CFLAGS) $(INC_PATH) -o $(LIB_PATH)/main.o
