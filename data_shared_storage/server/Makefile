OBJS = *.c
TARGET = data_server

$(TARGET) : $(OBJS)
	gcc -g -o $(TARGET) $(OBJS) -lpthread -lsqlite3

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
