export
CPP=clang
CPP=gcc
#CPP=icc
ifeq ($(CPP), gcc)
FASTMATH=-funroll-loops -ffast-math -fstrict-aliasing -fomit-frame-pointer -funsafe-loop-optimizations -mfpmath=sse
CFLAGS=-I../ -I./utils -std=c++11 -DGCC -DNDEBUG -m64 -O3 -march=native -Wall -Wno-strict-aliasing
LD=gcc
LFLAGS= -L/usr/local/lib -lstdc++ -O3 -lfftw3 -lm -march=native -m64 $(FASTMATH)
endif
ifeq ($(CPP), clang)
#clang generates significantly worse code than gcc for this project, but compiles faster
#so might as well make it a default debug build
FASTMATH=
CFLAGS=-I../ -I./utils -std=c++11 -g -DCLANG  -DDEBUG -m64 -Werror -W -Wall
LD=clang
LFLAGS= -L/usr/local/lib -lstdc++ -lfftw3 -lm 
endif
ifeq ($(CPP), icc)
export FASTMATH= -ansi-alias -fargument-noalias -fno-alias -funroll-loops -no-ansi-alias-check -complex-limited-range -scalar-rep -no-prec-div -finline-functions
export CFLAGS= -std=c++11 -fast -restrict -O3 -fpermissive -DICC -mkl
export LD=$(CPP)
export LFLAGS= -lfftw3 -lstdc++ -mkl
endif

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.hpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/
