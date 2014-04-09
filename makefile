#Main makefile for the laser code
#
#Sam Schetterer, Xing Fu, Steve Brunton
#University of Washington
#
#www.github.com/schets/lasers


OBJS = lib/rhs.o  lib/controller.o lib/integrator.o lib/main.o lib/engine.o lib/objective.o lib/simulation.o
include make.inc
.PHONY: all
all: integrator rhs controller parser objective simulation utils writer
	@mkdir -p bin
	@mkdir -p lib
	$(LD)  lib/*.o -o bin/lilac $(LFLAGS) 
	@cp bin/lilac rundir/lilac

.PHONY: integrator
integrator:
	@$(MAKE) -C integrator
.PHONY: rhs
rhs:
	@$(MAKE) -C rhs

.PHONY: controller
controller:
	@$(MAKE) -C controller

.PHONY: parser
parser:
	@$(MAKE) -C parser

.PHONY: objective
objective:
	@$(MAKE) -C objective

.PHONY: simulation
simulation:
	@$(MAKE) -C simulation
	
.PHONY: writer
writer:
	@$(MAKE) -C writer
	
.PHONY: utils
utils:
	@$(MAKE) -C utils
	
.PHONY: clean
clean:
	rm -rf lib/*.o bin/*
