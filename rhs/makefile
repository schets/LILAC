#makefile for the RHS class and derived variants

include ../make.inc

all: $(LIB_MAIN)/rhs.o

LIB_PATH= $(LIB_MAIN)/rhs

RHS_OBJ = $(LIB_PATH)/rhs_main.o $(LIB_PATH)/rhs_CNLS.o $(LIB_PATH)/rhs_SQGLE.o $(LIB_PATH)/example_rhs.o

$(LIB_MAIN)/rhs.o: $(RHS_OBJ)
	@rm -f $(LIB_MAIN)/rhs.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/rhs.o 
$(LIB_PATH)/rhs_main.o: rhs.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/rhs_main.o
	$(CPP) -c rhs.cpp $(INC_PATH)  -o $(LIB_PATH)/rhs_main.o $(CFLAGS)
$(LIB_PATH)/rhs_CNLS.o: rhs_CNLS.cpp rhs_type.h $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_CNLS.o
	$(CPP) -c rhs_CNLS.cpp $(INC_PATH)  -o $(LIB_PATH)/rhs_CNLS.o  $(CFLAGS) $(FASTMATH)
$(LIB_PATH)/example_rhs.o: example_rhs.cpp rhs_type.h $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/example_rhs.o
	$(CPP) -c example_rhs.cpp $(INC_PATH)  -o $(LIB_PATH)/example_rhs.o $(CFLAGS)
$(LIB_PATH)/rhs_SQGLE.o: rhs_SQGLE.cpp rhs_type.h $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_SQGLE.o
	$(CPP) -c rhs_SQGLE.cpp $(INC_PATH)  -o $(LIB_PATH)/rhs_SQGLE.o $(CFLAGS) $(FASTMATH) 
