CFLAGS=-std=c99 -O3 -ffast-math -ftree-vectorize -ftree-vectorizer-verbose=6
all: inteq
inteq: rhs.o main.o
	@echo "Linking binaries"
	gcc *.o -lgsl -lgslcblas -lfftw3 -lm -o inteq.out
main.o: main.c
	@echo "Compiling main routine"
	gcc  -c main.c $(CFLAGS) -o main.o
rhs.o: rhs.c
	@echo "Compiling RHS"
	gcc -c rhs.c $(CFLAGS) -o rhs.o

clean:
	rm -f *.o
