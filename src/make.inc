export
CPP=clang
CPP=gcc
#CPP=icc
ifeq ($(CPP), gcc)
FASTMATH=-ffast-math -fstrict-aliasing -fomit-frame-pointer
CFLAGS=-I../ -I./utils -std=c++0x -DGCC -DNDEBUG -m64 -O3 -march=native -mtune=native -Wall -Wno-strict-aliasing -pipe
LD=gcc
LFLAGS= -L/usr/local/lib -lstdc++ -O3 -lfftw3 -lm -march=native -m64 -pipe $(FASTMATH)
endif
ifeq ($(CPP), clang)
#clang generates significantly worse code than gcc for this project, but compiles faster
#so might as well make it a default debug build
FASTMATH=
CFLAGS=-I../ -I./utils -stdlib=libc++ -std=c++11 -g -DCLANG  -DDEBUG -m64 -Werror -W -Wall
LD=clang
LFLAGS= -L/usr/local/lib -lstdc++ -lfftw3 -lm 
endif
ifeq ($(CPP), icc)
export FASTMATH= -ansi-alias -fargument-noalias -fno-alias -no-ansi-alias-check -complex-limited-range -scalar-rep -no-prec-div -finline-functions
export CFLAGS= -ip -ipo -std=c++11 -fast -restrict -O3 -fpermissive -DICC -mkl=sequential
export LD=$(CPP)
export LFLAGS= -lstdc++ -mkl=sequential
endif

LIB_MAIN=../../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.hpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/
