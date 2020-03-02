MAIN_PATH = ..
MPATH = .
SRC = $(MPATH)/src
SOCK = $(MAIN_PATH)/sock_stream
OUTPUT_PATH = $(MAIN_PATH)/debug

CXX = g++
DEBUG = -g
CFLAGS = $(DEBUG) -c
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = tcp_server
LIBS = -lsock

HEADER_PATH = -I$(MPATH)/include -I$(SOCK)/include
LIB_PATH = -L$(SOCK)

$(TARGET) : $(OBJS)
	$(CXX) $^ -o $@ $(LIB_PATH) $(LIBS) 

$(OBJS) : %.o : %.c
	$(CXX) $(CFLAGS) $< -o $@ $(HEADER_PATH)

.PHONY : clean

clean :
	rm -rf $(TARGET) $(OBJS)
