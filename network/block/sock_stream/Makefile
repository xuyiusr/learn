MPATH = .
MAIN_PATH = ..
SRC = $(MPATH)/src
OUTPUT_PATH = $(MAIN_PATH)

# make a static library
AR = ar -rc
CXX = g++
DEBUG = -g
CFLAGS = $(DEBUG) -c
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = libsock.a

HEADER_PATH = -I$(MPATH)/include

$(TARGET) : $(OBJS)
	$(AR) $@ $(OBJS)

$(OBJS) : %.o : %.c
	$(CXX) $(CFLAGS) $< -o $@ $(HEADER_PATH)

.PHONY : clean copy

clean : 
	rm -rf $(TARGET) $(OBJS)
