CFLAGS=-I./ -I./utils #-O3 -ffast-math -march=native 
LFLAGS=-lfftw3 -lm
CPP=g++
LD=g++
all: bin/inteq
bin/inteq: utils lib/rhs.o lib/integrator.o lib/main.o 
	@echo "Linking binaries"
	$(LD)  lib/*.o $(LFLAGS) -o bin/inteq
#utils: utils/*.cpp
#	@echo "util"
#	@rm -f libs/*.o libs/*/*.o
#	make
lib/main.o: main.cpp utils/*.cpp
	@echo "Compiling main routine"
	@rm -f lib/main.o
	$(CPP) -c main.cpp $(CFLAGS) -o lib/main.o
lib/rhs.o: rhs/*.cpp utils/*.cpp
	@echo "Compiling RHS"
	@rm -f lib/rhs.o
	$(CPP) -c rhs/*.cpp $(CFLAGS)
	@mv *.o lib/rhs/
	@ld -r lib/rhs/*.o -o lib/rhs.o
lib/integrator.o: integrator/*.cpp utils/*.cpp
	@echo "Compiling Integrators"
	@rm -f lib/integrator.o
	$(CPP) -c integrator/*.cpp $(CFLAGS)
	@mv *.o lib/integrator/
	@ld -r lib/integrator/*.o -o lib/integrator.o
clean:
	rm -r -f *.o */*.o */*/*.o */*.a
