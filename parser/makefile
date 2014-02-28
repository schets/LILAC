include ../make.inc
LIB_PATH=$(LIB_MAIN)/engine
ENG_OBJ= $(LIB_PATH)/engine_main.o $(LIB_PATH)/item.o $(LIB_PATH)/engine_read.o $(LIB_PATH)/graph.o $(LIB_PATH)/engine_run.o
all: $(LIB_MAIN)/engine.o 

$(LIB_MAIN)/engine.o: $(ENG_OBJ)
	@rm -f $(LIB_MAIN)/engine.o
	ld -r $(LIB_PATH)/*.o -o $(LIB_MAIN)/engine.o
$(LIB_PATH)/engine_read.o:engine_read.cpp $(INLINE_HEADERS) 
	@rm -f $(LIB_PATH)/engine_read.o
	$(CPP) -c engine_read.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/engine_read.o
$(LIB_PATH)/graph.o:graph.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/graph.o
	$(CPP) -c graph.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/graph.o
$(LIB_PATH)/engine_main.o:engine.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/engine_main.o
	$(CPP) -c engine.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/engine_main.o
$(LIB_PATH)/item.o:item.cpp variable.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/item.o
	$(CPP) -c item.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/item.o
$(LIB_PATH)/engine_run.o: engine_run.cpp $(INLINE_HEADERS)
	@rm -f $(LIB_PATH)/engine_run.o
	$(CPP) -c engine_run.cpp $(INC_PATH) $(CFLAGS) -o $(LIB_PATH)/engine_run.o
