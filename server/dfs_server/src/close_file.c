#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

myDFS_int 
close_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	x, i, fd;
	myDFS_msg	msg;

	x = read(sockfd, &fd, sizeof(fd));
	if(x == -1){
		chk("reading socket");
		return -1;
	}

	printf("Request to close file %d\n", fd);
	x = close_fd(auth_ptr, fd);
	if(x == -1){
		chk("closing fd");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing socket");
			return -1;
		}
	}

	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	return x;
}


