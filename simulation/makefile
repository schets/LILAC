#makefile for the simulation class and derived variants

include ../make.inc

all: $(LIB_MAIN)/simulation.o

LIB_PATH= $(LIB_MAIN)/simulation

CONT_OBJ = $(LIB_PATH)/simulation.o $(LIB_PATH)/stable.o $(LIB_PATH)/jones_optical.o 

$(LIB_MAIN)/simulation.o: $(CONT_OBJ)
	@rm -f $(LIB_MAIN)/simulation.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/simulation.o 
$(LIB_PATH)/simulation.o: simulation.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/simulation.o
	$(CPP) -c simulation.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/simulation.o
$(LIB_PATH)/jones_optical.o: jones_optical.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/jones_optical.o
	$(CPP) -c jones_optical.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/jones_optical.o
$(LIB_PATH)/stable.o: stable.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/stable.o
	$(CPP) -c stable.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/stable.o

