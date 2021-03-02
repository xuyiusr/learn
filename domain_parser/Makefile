OBJS = *.c
TARGET = domain_parser

$(TARGET) : $(OBJS)
	gcc -g -o $(TARGET) $(OBJS) -lpthread

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
