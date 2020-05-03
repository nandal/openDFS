#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

int open_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	fd, x;
	myDFS_msg	msg;
	myDFS_OPEN_ARG	arg;
	myDFS_inode 	inode;

	x = read(sockfd, &arg, sizeof(arg));
	if(x == -1){
		chk("reading socket");
		return -1;
	}
	puts("args are read.");
	inode = get_inode_from_path(&arg.path);
	if(inode == -1){
		printf("Error in geting inode.\n");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		x = write(sockfd, &ERROR, sizeof(ERROR));
		return -1;
	}
	puts("inode is granted.");
	x = authorize(auth_ptr, arg.flags, inode);
	if(x == -1){
		printf("Error in authorization.\n");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		x = write(sockfd, &ERROR, sizeof(ERROR));
		return -1;
	}
	puts("Authorization success.");
	fd = get_fd(auth_ptr, inode, arg.flags);
	if(fd == -1){
		printf("Error in getting fd.\n");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		x = write(sockfd, &ERROR, sizeof(ERROR));
		return -1;
	}
	puts("FD is assigned.");
	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	puts("result msg is written.");
	x = write(sockfd, &fd, sizeof(fd));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	puts("fd is written.");
	return 0;	
}



















