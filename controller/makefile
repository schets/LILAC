#makefile for the controller class and derived variants

include ../make.inc

all: $(LIB_MAIN)/controller.o

LIB_PATH= $(LIB_MAIN)/controller

CONT_OBJ = $(LIB_PATH)/controller.o $(LIB_PATH)/toroidal.o

$(LIB_MAIN)/controller.o: $(CONT_OBJ)
	@rm -f $(LIB_MAIN)/controller.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/controller.o 
$(LIB_PATH)/controller.o: controller.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/controller.o
	$(CPP) -c controller.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/controller.o
$(LIB_PATH)/toroidal.o: toroidal.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/toroidal.o
	$(CPP) -c toroidal.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/toroidal.o


