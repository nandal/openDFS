# makefile for slave

slave: slave.o \
slave_process_request.o \
slave_read_config.o \
slave_make_entry_slave_list.o \
slave_read_data.o \
slave_write_data.o
	gcc -o slave \
	slave_process_request.o \
	slave_read_config.o \
	slave_make_entry_slave_list.o \
	slave_read_data.o \
	slave_write_data.o \
	slave.o

slave.o: slave.c \
header/slave.h
	gcc -c slave.c

slave_process_request.o: slave_process_request.c \
header/slave.h 
	gcc -c slave_process_request.c

slave_read_config.o: slave_read_config.c \
header/slave.h
	gcc -c slave_read_config.c

slave_make_entry_slave_list.o: slave_make_entry_slave_list.c \
header/slave.h
	gcc -c slave_make_entry_slave_list.c

slave_read_data.o: slave_read_data.c \
header/slave.h
	gcc -c slave_read_data.c

slave_write_data.o: slave_write_data.c \
header/slave.h
	gcc -c slave_write_data.c

clean:
	rm *.o

cleanall:
	find /HD/storage/project/DFS/ -name '*.o' -delete
