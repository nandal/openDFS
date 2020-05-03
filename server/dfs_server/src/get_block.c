#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/dfs_server.h"

myDFS_int 
get_block(const myDFS_dnode_s	* dnode_ptr){
	myDFS_int	fd, sockfd, x, i;
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_char	file[MAX_PATH_SIZE];
	myDFS_msg	msg;
	myDFS_fhead_s	fhead;
	myDFS_size	size;
	
	struct sockaddr_in address;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		chk("socket creation");
		return -1;
	}
	msg = myDFS_MSG_READ;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(slaves[dnode_ptr->slave_id].ip);
	address.sin_port = htons(slaves[dnode_ptr->slave_id].port);


	x = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if(x == -1){
		chk("connect");
		ERROR = myDFS_ESCONNECT;
		close(sockfd);
		return -1;
	}
	
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		ERROR = myDFS_ESWRITE;
		close(sockfd);
		return -1;
	}

	sprintf(file, "%s/%i/%u.txt", SERVER_WAREHOUSE, dnode_ptr->slave_id, dnode_ptr->sub_id);
	fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("opening data_block warehouse");
		return -1;
	}
	
	x = write(sockfd, &(dnode_ptr->sub_id), sizeof(dnode_ptr->sub_id));
	if(x == -1){
		chk("writing to socket");
		ERROR = myDFS_ESWRITE;
		close(sockfd);
		return -1;
	}

	size = 0;
	while(x = read(sockfd, data, myDFS_BUFSIZ)){
		x = write(fd, data, x);
	}
	puts("data block is collected from slave.");

	close(fd);
	close(sockfd);
	return 0;
}
