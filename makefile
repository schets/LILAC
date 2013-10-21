CFLAGS=-std=c99 -O3
all: inteq
inteq: rhs.o main.o
	@echo "Linking binaries"
	gcc *.o -lgsl -lgslcblas -lfftw3 -lm -o inteq
main.o:
	@echo "Compiling main routine"
	gcc  -c main.c $(CFLAGS) -o main.o
rhs.o:
	@echo "Compiling RHS"
	gcc -c rhs.c $(CFLAGS) -o rhs.o

clean:
	rm -f *.o
