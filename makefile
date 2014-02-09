#Main makefile for the laser code
#
#Sam Schetterer, Xing Fu, Steve Brunton
#University of Washington
#
#www.github.com/schets/lasers


OBJS = lib/rhs.o  lib/controller.o lib/integrator.o lib/main.o lib/engine.o lib/objective.o lib/simulation.o
include make.inc
all: $(OBJS) bin/lilac
.PHONY: clean
bin/lilac: $(OBJS) 
	@mkdir -p bin
	$(LD)  lib/*.o $(LFLAGS) -o bin/lilac
lib/main.o: main.cpp utils/*.cpp
	@rm -f lib/main.o
	$(CPP) -c main.cpp $(CFLAGS) -o lib/main.o
lib/rhs.o: rhs/*.cpp utils/*cpp 
	@mkdir -p lib
	@mkdir -p lib/rhs
	@rm -f lib/rhs.o
	@$(MAKE) -C rhs
lib/integrator.o: integrator/*.cpp utils/*.cpp
	@mkdir -p lib/integrator
	@rm -f lib/integrator.o
	@$(MAKE) -C integrator
lib/controller.o: controller/*.cpp utils/*.cpp
	@mkdir -p lib/controller
	@rm -f lib/controller.o
	@$(MAKE) -C controller
lib/engine.o: parser/*.cpp utils/*.cpp parser/*.c
	@mkdir -p lib/engine
	@rm -f lib/engine.o
	@$(MAKE) -C parser
lib/objective.o: objective/*.cpp utils/*.cpp
	@mkdir -p lib/objective
	@rm -f lib/objective.o
	@$(MAKE) -C objective
lib/simulation.o: simulation/*.cpp utils/*.cpp
	@mkdir -p lib/simulation
	@rm -f lib/simulation.o
	@$(MAKE) -C simulation
clean:
	rm -rf lib/* bin/*
