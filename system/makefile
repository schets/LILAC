#makefile for the system class and derived variants

include ../make.inc

all: $(LIB_MAIN)/system.o

LIB_PATH= $(LIB_MAIN)/system

CONT_OBJ = $(LIB_PATH)/system.o $(LIB_PATH)/jones_optical.o 

$(LIB_MAIN)/system.o: $(CONT_OBJ)
	@rm -f $(LIB_MAIN)/system.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/system.o 
$(LIB_PATH)/system.o: system.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/system.o
	$(CPP) -c system.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/system.o
$(LIB_PATH)/jones_optical.o: jones_optical.cpp $(INLINE_HEADERS)
	@mkdir -p $(LIB_PATH)
	@rm -f $(LIB_PATH)/jones_optical.o
	$(CPP) -c jones_optical.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/jones_optical.o

