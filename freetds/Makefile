OBJS = main.c
TARGET = connect_sqlserver

$(TARGET) : $(OBJS)
	gcc -o $(TARGET) $(OBJS) -L/usr/local/freetds/lib -lsybdb -I/usr/local/freetds/include /home/audit/lib/libsybdb.so.5 -Wl,-rpath,/home/audit/lib

.PHONY : clean
clean :
	rm edit $(objects)
