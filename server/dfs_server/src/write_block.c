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
write_block(const myDFS_dnode_s	* dnode_ptr){
	myDFS_int	fd, sockfd, x;
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_char	file[MAX_PATH_SIZE];
	myDFS_msg	msg;
	myDFS_fhead_s	fhead;
	
	struct sockaddr_in address;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		chk("socket creation");
		return -1;
	}
	msg = myDFS_MSG_WRITE;
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
	fd = open(file, O_RDONLY);
	if(fd == -1){
		chk("opening data_block warehouse");
		return -1;
	}
	
	sprintf(fhead.name, "%u.txt", dnode_ptr->sub_id);
	fhead.size = lseek(fd, 0, SEEK_END);	
	x = write(sockfd, &fhead, sizeof(fhead));
	if(x == -1){
		chk("writing to socket");
		ERROR = myDFS_ESWRITE;
		close(sockfd);
		return -1;
	}
	
	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}
	if(msg == myDFS_MSG_SUCCESS){
		puts("fhead is successfully sent.");
	}else{
		puts("failure after sending fhead");
	}
	
	x = lseek(fd, 0, SEEK_SET);
	do{
		x = read(fd, data, myDFS_BUFSIZ);
		x = write(sockfd, data, x);
	}while(x > 0);

	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}
	if(msg == myDFS_MSG_SUCCESS){
		puts("block is successfully written to slave.");
		unlink(file);
	}else{
		puts("block writing is failed.");
	}	

	close(fd);
	close(sockfd);
	return 0;
}
