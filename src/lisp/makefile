include ../make.inc
LIB_PATH=$(LIB_MAIN)
ENG_OBJ= $(LIB_PATH)/engine_read.o $(LIB_PATH)/graph.o $(LIB_PATH)/eval_lisp.o 

all: $(ENG_OBJ)


$(LIB_PATH)/engine_read.o:engine_read.cpp $(INLINE_HEADERS) 
	@rm -f $(LIB_PATH)/engine_read.o
	$(CPP) -c engine_read.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/engine_read.o

$(LIB_PATH)/graph.o:graph.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/graph.o
	$(CPP) -c graph.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/graph.o


$(LIB_PATH)/eval_lisp.o:eval_lisp.cpp $(INLINE_HEADERS) 
	@rm -f $(LIB_PATH)/eval_lisp.o
	$(CPP) -c eval_lisp.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/eval_lisp.o


.PHONY: clean

clean:
	rm -f $(ENG_OBJ)
