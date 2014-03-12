export
CPP=gcc
ifeq ($(CPP), gcc)
FASTMATH=-funroll-loops -ffast-math -funsafe-loop-optimizations -fstrict-aliasing -O3 
CFLAGS=-I../ -I./utils -g -std=c++03 -DGCC -m64 -O3 -march=native
LD=gcc
LFLAGS= -lstdc++ -lfftw3 -lm 
endif
ifeq ($(CPP), CC)
export FAST_FLAGS= -ansi-alias -fargument-noalias -fno-alias -funroll-loops -no-ansi-alias-check -complex-limited-range -scalar-rep -no-prec-div -finline-functions
export CFLAGS= -fast -no-ipo -restrict -O3  -fpermissive -DICC -mkl=sequential
export LD=CC
export LFLAGS= -lfftw3 -mkl=sequential
endif

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/
#export FAST_FLAGS= -ansi-alias -fargument-noalias -fno-alias -no-ansi-alias-check -complex-limited-range -guide-vec -scalar-rep -no-prec-div
