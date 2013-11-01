CFLAGS=-O3 -ffast-math -march=native -I./
LFLAGS=-lfftw3 -lm
CPP=g++
LD=g++
all: bin/inteq
bin/inteq: lib/rhs.o lib/integrator.o lib/main.o
	@echo "Linking binaries"
	@mkdir -p bin
	$(LD)  lib/*.o $(LFLAGS) -o bin/inteq
lib/main.o: main.cpp
	@echo "Compiling main routine"
	@mkdir -p lib
	$(CPP) -c main.cpp $(CFLAGS) -o lib/main.o
lib/rhs.o: rhs/*.cpp 
	@echo "Compiling RHS"
	$(CPP) -c rhs/*.cpp $(CFLAGS)
	@mkdir -p lib
	@mv *.o lib/rhs/
	@ld -r lib/rhs/*.o -o lib/rhs.o
lib/integrator.o: integrator/*.cpp
	@echo "Compiling Integrators"
	$(CPP) -c integrator/*.cpp $(CFLAGS)
	@mkdir -p lib
	@mv *.o lib/integrator/
	@ld -r lib/integrator/*.o -o lib/integrator.o
clean:
	rm -r -f *.o */*.o */*/*.o */*.a
