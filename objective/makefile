#makefile for the controller class and derived variants

include ../make.inc

all: $(LIB_MAIN)/objective.o

LIB_PATH= $(LIB_MAIN)/objective

CONT_OBJ = $(LIB_PATH)/objective.o $(LIB_PATH)/cnls_pulse.o

$(LIB_MAIN)/objective.o: $(CONT_OBJ)
	@rm -f $(LIB_MAIN)/objective.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/objective.o 
$(LIB_PATH)/objective.o: objective.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/objective.o
	$(CPP) -c objective.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/objective.o
$(LIB_PATH)/cnls_pulse.o: cnls_pulse.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/cnls_pulse.o
	$(CPP) -c cnls_pulse.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/cnls_pulse.o


