#makefile for the simulation class and derived variants

include ../make.inc


LIB_PATH= $(LIB_MAIN)

SIM_OBJ = $(LIB_PATH)/simulation.o $(LIB_PATH)/stable.o $(LIB_PATH)/stable_ode.o $(LIB_PATH)/jones_optical.o $(LIB_PATH)/ode.o 

S_HEADS= stable_ode_tmpl.hpp stable_spectral_pde_1d_tmpl.hpp

all: $(SIM_OBJ) 

$(LIB_PATH)/simulation.o: simulation.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/simulation.o
	$(CPP) -c simulation.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/simulation.o

$(LIB_PATH)/jones_optical.o: jones_optical.cpp $(S_HEADS)  $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/jones_optical.o
	$(CPP) -c jones_optical.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/jones_optical.o

$(LIB_PATH)/stable.o: stable.cpp ../writer/data.hpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/stable.o
	$(CPP) -c stable.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/stable.o

$(LIB_PATH)/stable_ode.o: stable_ode.cpp stable_ode_tmpl.hpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/stable_ode.o
	$(CPP) -c stable_ode.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/stable_ode.o

$(LIB_PATH)/ode.o: ode.cpp ode_tmpl.hpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/ode.o
	$(CPP) -c ode.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/ode.o

.PHONY: clean

clean:
	rm -f $(SIM_OBJ)
