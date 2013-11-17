export
CFLAGS=-I./ -I./utils -O3 -ffast-math 
LFLAGS= -lfftw3 -lm
CPP=g++
LD=g++
LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/

