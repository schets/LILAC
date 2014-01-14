#Main makefile for the laser code
#
#Sam Schetterer, Xing Fu, Steve Brunton
#University of Washington
#
#www.github.com/schets/lasers


OBJS = lib/rhs.o  lib/solver.o lib/integrator.o lib/main.o
all: $(OBJS) bin/inteq
	@cd parser && make
include make.inc
.PHONY: clean
bin/inteq: $(OBJS) 
	@mkdir -p bin
	$(LD)  lib/*.o $(LFLAGS) -o bin/inteq
lib/main.o: main.cpp utils/*.cpp
	@rm -f lib/main.o
	$(CPP) -c main.cpp $(CFLAGS) -o lib/main.o
lib/rhs.o: rhs/*.cpp utils/*cpp 
	@mkdir -p lib
	@mkdir -p lib/rhs
	@$(MAKE) -C rhs
lib/integrator.o: integrator/*.cpp utils/*.cpp
	@mkdir -p lib/integrator
	@rm -rf lib/integrator.o
	@$(MAKE) -C integrator
lib/solver.o:
clean:
	rm -rf lib/* bin/*
