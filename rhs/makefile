#makefile for the RHS class and derived variants

include ../make.inc

all: $(LIB_MAIN)/rhs.o

LIB_PATH= $(LIB_MAIN)/rhs

RHS_OBJ = $(LIB_PATH)/rhs_main.o $(LIB_PATH)/rhs_CNLS.o


$(LIB_MAIN)/rhs.o: $(RHS_OBJ)
	@rm -f $(LIB_MAIN)/rhs.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/rhs.o 
$(LIB_PATH)/rhs_main.o: rhs.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_main.o
	$(CPP) -c rhs.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/rhs_main.o
$(LIB_PATH)/rhs_CNLS.o: rhs_CNLS.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_CNLS.o
	$(CPP) -c rhs_CNLS.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/rhs_CNLS.o

