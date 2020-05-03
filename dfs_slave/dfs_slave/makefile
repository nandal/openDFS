all:
	gcc -c src/*.c
	mv *.o src/
	ar crv lib/libdfs_slave.a src/*.o

clean:
	rm src/*.o
