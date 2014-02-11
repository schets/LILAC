export
#these have minor changes to the output, mostly just slight shifting or changing of stable pulses
CPP=gcc
LD=gcc
ifeq ($(CPP), gcc)
FASTMATH = -funroll-loops -ffast-math -funsafe-loop-optimizations
CFLAGS=-I./ -I./utils -g -std=c++03 -O3 $(FASTMATH) -fstrict-aliasing -march=native -DGCC
endif
ifeq ($(CPP), icc)
CFLAGS= -restrict -O3 -ansi-alias -no-prec-div -fargument-noalias -fno-alias -finline-functions -funroll-loops -fpermissive -DICC
endif
LFLAGS= -lstdc++ -lfftw3 -lm 

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/

