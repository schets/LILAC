#makefile for the integrator class and derived variants
include ../make.inc
LIB_MAIN=../lib

LIB_PATH=$(LIB_MAIN)

INT_OBJ =  $(LIB_PATH)/rk45.o $(LIB_PATH)/integrator.o #$(LIB_PATH)/rk45_double.o $(LIB_PATH)/rk4.o

all: $(INT_OBJ)



$(LIB_PATH)/rk4.o: rk4.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rk4.o
	$(CPP) -c rk4.cpp $(INC_PATH) $(FASTMATH) $(CFLAGS) -o $(LIB_PATH)/rk4.o
$(LIB_PATH)/integrator.o: integrator.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/integrator.o
	$(CPP) -c integrator.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/integrator.o
$(LIB_PATH)/rk45.o: rk45.cpp rk45_tmpl.h $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rk45.o
	$(CPP) -c rk45.cpp $(INC_PATH) $(FASTMATH) $(CFLAGS) -o $(LIB_PATH)/rk45.o
$(LIB_PATH)/lsoda.o: lsoda.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/lsoda.o
	$(CPP) -c lsoda.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/lsoda.o 

