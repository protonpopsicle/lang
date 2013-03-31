.PHONY: all clean

all: vm compiler

vm: 
	$(MAKE) -C src/vm

compiler:
	$(MAKE) -C src/compiler

clean:
	cd src/vm && $(MAKE) clean	
	cd src/compiler && $(MAKE) clean
