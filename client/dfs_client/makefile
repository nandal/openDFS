# makefile for dfs_client lib
all:
	gcc src/*.c -c
	mv *.o src/
	ar crv lib/libdfs_client.a src/*.o
lib:
	ar crv lib/libdfs_client.a src/*.o

clean:
	rm src/*.o

