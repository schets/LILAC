#makefile in upper directory for convinience
.PHONY: all
all:
	$(MAKE) -C src

.PHONY: clean
clean:
	rm lib/* bin/* -rf
