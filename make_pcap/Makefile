OBJS = *.c
TARGET = make_pcap

$(TARGET) : $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lpcap

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
