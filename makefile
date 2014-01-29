#Main makefile for the laser code
#
#Sam Schetterer, Xing Fu, Steve Brunton
#University of Washington
#
#www.github.com/schets/lasers


OBJS = lib/rhs.o  lib/controller.o lib/integrator.o lib/main.o lib/engine.o
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
	@rm -rf lib/rhs.o
	@$(MAKE) -C rhs
lib/integrator.o: integrator/*.cpp utils/*.cpp
	@mkdir -p lib/integrator
	@rm -rf lib/integrator.o
	@$(MAKE) -C integrator
lib/controller.o: controller/*.cpp utils/*.cpp
	@mkdir -p lib/controller
	@rm -rf lib/controller.o
	@$(MAKE) -C controller
lib/engine.o: parser/*.cpp utils/*.cpp
	@mkdir -p lib/engine
	@rm -rf lib/engine.o
	@$(MAKE) -C parser

clean:
	rm -rf lib/* bin/*
