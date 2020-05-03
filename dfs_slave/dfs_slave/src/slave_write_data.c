#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "header/slave.h"

myDFS_size slave_write_data(int sockfd){
	myDFS_int	fd;
	myDFS_int	x=0, j;
	myDFS_size	i=0;
	myDFS_fhead_s	filehead;
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_char 	filename[50];
	myDFS_msg_s	msg;

	bzero(filename, 50);
	x = read(sockfd, &filehead, sizeof(myDFS_fhead_s));
	sprintf(filename, "warehouse/%s", filehead.name);
	unlink(filename);
	fd = open(filename, O_WRONLY|O_CREAT , 0666);
	msg.type = myDFS_MSG_SUCCESS;
	write(sockfd, &msg, sizeof(myDFS_msg_s));

	if(fd == -1){
		printf("ERROR:slave_write_data.c:12\n");
		perror("creating file in warehouse");
		exit(-1);
	}
	
	i = 0;
	for(j = 0; j < myDFS_CYCLES; j++){
		bzero(data, myDFS_BUFSIZ);
		x = read(sockfd, data, myDFS_BUFSIZ);
		write(fd, data, x);
		write(1, data, x);
		i += x;
		if(i >= filehead.size)break;
	}
	printf("%i of bytes are read and written in %s\n", i, filename);
	close(fd);
	return i;
}
