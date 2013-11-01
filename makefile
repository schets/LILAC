CFLAGS= -O3 -ffast-math -I./
LFLAGS= -lfftw3 -lm
all: inteq
inteq: rhs.a integrator.a main.o
	@echo "Linking binaries"
	g++ lib/*.o  $(LFLAGS) -o bin/inteq
main.o: main.cpp
	@echo "Compiling main routine"
	g++ -c main.cpp $(CFLAGS) -o lib/main.o
rhs.a: rhs/*.cpp 
	@echo "Compiling RHS"
	g++ -c rhs/*.cpp $(CFLAGS)
	mv *.o lib/
integrator.a: integrator/*.cpp
	@echo "Compiling Integrators"
	g++ -c integrator/*.cpp $(CFLAGS)
	mv *.o lib/
clean:
	rm -r -f *.o */*.o
