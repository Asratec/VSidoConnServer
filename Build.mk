.PHONY:install doc service build clean setup


PACKROOT ?= $(shell pwd)/install
OBJROOT  := $(shell pwd)/objects
SRCROOT  := $(shell pwd)/


#build:websockets pre-build
build: websockets pre-build
	cd $(OBJROOT) && make
	
pre-build:
	mkdir -p $(OBJROOT)
	cd $(OBJROOT) && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PACKROOT)  $(SRCROOT)\
	

install: websockets pre-build
	cd $(OBJROOT) && make install

websockets:
	mkdir -p $(OBJROOT)/websockets/
	cd $(OBJROOT)/websockets/ && cmake -DCMAKE_INSTALL_PREFIX=$(PACKROOT) $(SRCROOT)/3dpart/websockets\
		-DLWS_WITH_SSL=OFF\
		-DLWS_IPV6=OFF
	cd $(OBJROOT)/websockets/ && make install

clean:
	rm -rf $(PACKROOT) $(OBJROOT)

