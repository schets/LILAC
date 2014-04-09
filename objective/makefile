#makefile for the controller class and derived variants

include ../make.inc


LIB_PATH= $(LIB_MAIN)

OBJ_OBJ = $(LIB_PATH)/objective.o $(LIB_PATH)/n_pulse_score.o

all: $(OBJ_OBJ)


$(LIB_PATH)/objective.o: objective.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/objective.o
	$(CPP) -c objective.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/objective.o

$(LIB_PATH)/n_pulse_score.o: n_pulse_score.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/n_pulse_score.o
	$(CPP) -c n_pulse_score.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/n_pulse_score.o

.PHONY: clean

clean:
	rm -f $(OBJ_OBJ)
