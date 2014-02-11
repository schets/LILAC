export
#these have minor changes to the output, mostly just slight shifting or changing of stable pulses
CPP=CC
ifeq ($(CPP), gcc)
FASTMATH = -funroll-loops -ffast-math -funsafe-loop-optimizations
CFLAGS=-I./ -I./utils -g -std=c++03 -O3 $(FASTMATH) -fstrict-aliasing -march=native -DGCC
LD=gcc
LFLAGS= -lfftw -lm 
endif
ifeq ($(CPP), CC)
CFLAGS= -I./ -I./utils -mkl=sequential -fast -no-ipo -restrict -O3 -ansi-alias -no-prec-div -fargument-noalias -fno-alias -finline-functions -funroll-loops -fpermissive -DICC
LD=CC
LFLAGS= -mkl=sequential -lfftw3 -lm 
endif

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/

