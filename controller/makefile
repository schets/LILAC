#makefile for the controller class and derived variants

include ../make.inc

LIB_PATH= $(LIB_MAIN)

CONT_OBJ = $(LIB_PATH)/controller.o $(LIB_PATH)/toroidal.o

all: $(CONT_OBJ)


$(LIB_PATH)/controller.o: controller.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/controller.o
	$(CPP) -c controller.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/controller.o

$(LIB_PATH)/toroidal.o: toroidal.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/toroidal.o
	$(CPP) -c toroidal.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/toroidal.o


