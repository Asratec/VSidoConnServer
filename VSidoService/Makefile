.PHONY:all clean install

SRC          := main.cpp
SRC          += uart_read.cpp
SRC          += uart_send.cpp
SRC          += dispatch.cpp
SRC          += rs_request.cpp
SRC          += rs_response.cpp
SRC          += ws_request.cpp
SRC          += ws_response.cpp
SRC          += $(shell find vsido_request_*.cpp  -type f)
SRC          += $(shell find vsido_response_*.cpp  -type f)


OBJs         := $(patsubst %.cpp,%.o,$(SRC))

CXXFLAGS     := -std=c++11
CXXFLAGS     += -g
#CXXFLAGS     += -I../prebuild/websockets/include
#CXXFLAGS    += -Os
CXXFLAGS     += -pthread

#LDFLAGS      :=  -L../prebuild/websockets/lib
LDFLAGS      += -lpthread 
LDFLAGS      += -lcrypto
LDFLAGS      += -lwebsockets 
LDFLAGS      += -lz



all: $(OBJs)
	g++  $(OBJs) $(LDFLAGS) -o VSido.srv
clean:
	rm -rf VSido.*
	rm -f *.o

.cpp.o:
	g++ $(CXXFLAGS) -c $<

install:all
	cp -f VSido.srv /home/vsido/bin/
	cp -f btsetup /home/vsido/bin/
uninstall:
	rm -f /home/vsido/bin/VSido.*
	rm -f /home/vsido/bin/btsetup
