#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include "header/slave.h"

myDFS_size slave_read_data(myDFS_int sockfd){
	myDFS_int	fd, x, j=0;
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_size	i = 0;
	myDFS_offset	offset;
	myDFS_char	filename[50];

	read(sockfd, &offset, sizeof(myDFS_offset));
	sprintf(filename, "warehouse/%u.txt", offset);
	fd = open(filename, O_RDONLY);
	while(x = read(fd, data, myDFS_BUFSIZ)){
		write(sockfd, data, x);
		i += x;
		write(1, data, x);
	}
	printf("\n%i of bytes are read \n", i);
	close(fd);
	return i;
}
