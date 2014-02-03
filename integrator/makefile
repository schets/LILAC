#makefile for the integrator class and derived variants
include ../make.inc
LIB_MAIN=../lib
all: $(LIB_MAIN)/integrator.o

LIB_PATH=$(LIB_MAIN)/integrator

INT_OBJ = $(LIB_PATH)/rk4.o $(LIB_PATH)/rk45.o $(LIB_PATH)/integrator.o $(LIB_PATH)/lsoda.o

$(LIB_MAIN)/integrator.o: $(INT_OBJ)
	@rm -f $(LIB_MAIN)/integrator.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/integrator.o 

$(LIB_PATH)/rk4.o: rk4.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rk4.o
	$(CPP) -c rk4.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/rk4.o
$(LIB_PATH)/integrator.o: integrator.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/integrator.o
	$(CPP) -c integrator.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/integrator.o
$(LIB_PATH)/rk45.o: rk45.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/rk45.o
	$(CPP) -c rk45.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/rk45.o
$(LIB_PATH)/lsoda.o: lsoda.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/lsoda.o
	$(CPP) -c lsoda.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/lsoda.o 
