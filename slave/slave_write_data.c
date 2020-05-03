#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "header/slave.h"

myDFS_size slave_write_data(int sockfd){
	myDFS_int	fd;
	myDFS_int	x=0, j;
	myDFS_size	i=0;
	myDFS_fhead_s	fhead;
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_char 	file[MAX_PATH_SIZE];
	myDFS_msg	msg;

	x = read(sockfd, &fhead, sizeof(fhead));
	sprintf(file, "%s/%s", SLAVE_WAREHOUSE, fhead.name);
	fd = open(file, O_WRONLY|O_CREAT|O_TRUNC , 0666);

	msg = myDFS_MSG_SUCCESS;
	write(sockfd, &msg, sizeof(msg));

	if(fd == -1){
		chk("creating file in warehouse");
		exit(-1);
	}
	
	i = 0;
	for(j = 0; j < myDFS_CYCLES; j++){
		bzero(data, myDFS_BUFSIZ);
		x = read(sockfd, data, myDFS_BUFSIZ);
		write(fd, data, x);
		write(1, data, x);
		i += x;
		if(i >= fhead.size)break;
	}
	printf("%i of bytes are read and written in %s\n", i, file);
	close(fd);
	return i;
}
