CFLAGS=-O3 -ffast-math -march=native -I./
LFLAGS=-lfftw3 -lm
CPP=g++
LD=g++
all: mkdirs bin/inteq
mkdirs:
	@mkdir -p lib
	@mkdir -p lib/integrator
	@mkdir -p lib/rhs
	@mkdir -p lib/solver
	@mkdir -p bin
bin/inteq: lib/rhs.o lib/integrator.o lib/main.o
	@echo "Linking binaries"
	$(LD)  lib/*.o $(LFLAGS) -o bin/inteq
lib/main.o: main.cpp
	@echo "Compiling main routine"
	$(CPP) -c main.cpp $(CFLAGS) -o lib/main.o
lib/rhs.o: rhs/*.cpp 
	@echo "Compiling RHS"
	$(CPP) -c rhs/*.cpp $(CFLAGS)
	@mv *.o lib/rhs/
	@ld -r lib/rhs/*.o -o lib/rhs.o
lib/integrator.o: integrator/*.cpp
	@echo "Compiling Integrators"
	$(CPP) -c integrator/*.cpp $(CFLAGS)
	@mv *.o lib/integrator/
	@ld -r lib/integrator/*.o -o lib/integrator.o
clean:
	rm -r -f *.o */*.o */*/*.o */*.a
