#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "header/slave.h"
int main(int argc, char *argv[]){
	char  data[myDFS_BUFSIZ];
	int fd, x, i;
	fd = open(argv[1], O_RDONLY);
	while(x = read(fd, data, myDFS_BUFSIZ))write(1, data, x);
	return 0;
}
