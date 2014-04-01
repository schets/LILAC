#Main makefile for the laser code
#
#Sam Schetterer, Xing Fu, Steve Brunton
#University of Washington
#
#www.github.com/schets/lasers


OBJS = lib/rhs.o  lib/controller.o lib/integrator.o lib/main.o lib/engine.o lib/objective.o lib/simulation.o
include make.inc
.PHONY: all
all:
	@mkdir -p bin
	@mkdir -p lib
	@$(MAKE) -C integrator
	@$(MAKE) -C rhs
	@$(MAKE) -C controller
	@$(MAKE) -C parser
	@$(MAKE) -C objective
	@$(MAKE) -C simulation
	@$(MAKE) -C utils
	$(LD)  lib/*.o -o bin/lilac $(LFLAGS) 
	@cp bin/lilac rundir/lilac

.PHONY: clean
clean:
	rm -rf lib/* bin/*
