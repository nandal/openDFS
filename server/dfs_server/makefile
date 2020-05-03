all:
	gcc -c src/*.c
	mv *.o src/
	ar crv lib/libdfs_server.a src/*.o

clean:
	rm src/*.o
