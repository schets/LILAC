#makefile for the writer class and derived variants

include ../make.inc

all: $(LIB_MAIN)/writer.o

LIB_PATH= $(LIB_MAIN)/writer

CONT_OBJ = $(LIB_PATH)/writer.o 
$(LIB_MAIN)/writer.o: $(CONT_OBJ)
	@rm -f $(LIB_MAIN)/writer.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/writer.o 
$(LIB_PATH)/writer.o: writer.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/writer.o
	$(CPP) -c writer.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/writer.o

