export
CPP=gcc
ifeq ($(CPP), gcc)
FASTMATH=-funroll-loops -ffast-math -funsafe-loop-optimizations -fstrict-aliasing -fomit-frame-pointer 
CFLAGS=-I../ -I./utils -std=c++11 -g -DGCC  -m64 -O3 -march=native -Wall -Werror
LD=gcc
LFLAGS= -L/usr/local/lib -lstdc++ -O3 -lfftw3 -lm 
endif
ifeq ($(CPP), CC)
export FAST_FLAGS= -ansi-alias -fargument-noalias -fno-alias -funroll-loops -no-ansi-alias-check -complex-limited-range -scalar-rep -no-prec-div -finline-functions
export CFLAGS= -fast -no-ipo -restrict -O3  -fpermissive -DICC -mkl=sequential
export LD=CC
export LFLAGS= -lfftw3 -mkl=sequential
endif

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.hpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/
