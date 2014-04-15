#makefile for the RHS class and derived variants

include ../make.inc

LIB_PATH= $(LIB_MAIN)

RHS_OBJ = $(LIB_PATH)/rhs_main.o $(LIB_PATH)/rhs_CNLS.o $(LIB_PATH)/rhs_SQGLE.o $(LIB_PATH)/example_rhs.o $(LIB_PATH)/rhs_sde.o $(LIB_PATH)/c_elegans.o

all: $(RHS_OBJ)

$(LIB_PATH)/rhs_main.o: rhs.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_main.o
	$(CPP) -c rhs.cpp $(INC_PATH)  -o $(LIB_PATH)/rhs_main.o $(CFLAGS)

$(LIB_PATH)/rhs_CNLS.o: rhs_CNLS.cpp rhs_type.hpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_CNLS.o
	$(CPP) -c rhs_CNLS.cpp $(INC_PATH)  -o $(LIB_PATH)/rhs_CNLS.o  $(CFLAGS) $(FASTMATH)

$(LIB_PATH)/example_rhs.o: example_rhs.cpp rhs_type.hpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/example_rhs.o
	$(CPP) -c example_rhs.cpp $(INC_PATH)  -o $(LIB_PATH)/example_rhs.o $(CFLAGS)

$(LIB_PATH)/rhs_SQGLE.o: rhs_SQGLE.cpp rhs_type.hpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_SQGLE.o
	$(CPP) -c rhs_SQGLE.cpp $(INC_PATH)  -o $(LIB_PATH)/rhs_SQGLE.o $(CFLAGS) $(FASTMATH) 

$(LIB_PATH)/rhs_sde.o: rhs_sde.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rhs_sde.o
	$(CPP) -c rhs_sde.cpp $(INC_PATH)  -o $(LIB_PATH)/rhs_sde.o $(CFLAGS)

$(LIB_PATH)/c_elegans.o: c_elegans.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/c_elegans.o
	$(CPP) -c c_elegans.cpp $(INC_PATH)  -o $(LIB_PATH)/c_elegans.o $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(RHS_OBJ)
