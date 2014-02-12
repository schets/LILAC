export
CPP=CC
ifeq ($(CPP), gcc)
FASTMATH=-funroll-loops -ffast-math -funsafe-loop-optimizations
CFLAGS=-I../ -I./utils -g -std=c++03 -O3 $(FASTMATH) -fstrict-aliasing -march=native -DGCC
LD=gcc
LFLAGS= -lstdc++ -lfftw3 -lm 
endif
ifeq ($(CPP), CC)
export CFLAGS= -fast -no-ipo -restrict -O3 -ansi-alias -no-prec-div -fargument-noalias -fno-alias -finline-functions -funroll-loops -fpermissive -DICC -mkl=sequential
export LD=CC
export LFLAGS= -lfftw3 -lm -mkl=sequential
endif

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/

