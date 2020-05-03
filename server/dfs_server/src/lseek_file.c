#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

int lseek_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	x, i;
	myDFS_msg	msg;
	myDFS_LSEEK_ARG	arg;
	myDFS_offset	offset;
	myDFS_int 	fd_index;

	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}

	x = read(sockfd, &arg, sizeof(arg));
	if(x == -1){
		chk("reading socket");
		return -1;
	}
	puts("args are read.");

	fd_index = logged_pa[auth_ptr->uid]->fd_index[arg.fd];
	printf("fd %d, offset %d ref %d\n", arg.fd, arg.offset, arg.ref);
	if(fd_global_pa[fd_index]->offset + arg.offset < 0){
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing socket");
		}
		x = write(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			chk("writing socket");
		}
		return -1;
	}else{
		msg = myDFS_MSG_SUCCESS;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing socket");
		}
		if(arg.ref == myDFS_SEEK_CUR){
			fd_global_pa[fd_index]->offset += arg.offset;
		}else if(arg.ref == myDFS_SEEK_SET){
			fd_global_pa[fd_index]->offset = arg.offset;
		}else if(arg.ref == myDFS_SEEK_END){
			fd_global_pa[fd_index]->offset += arg.offset;
		}
	
		printf("offset %u\n", fd_global_pa[fd_index]->offset);
		offset = fd_global_pa[fd_index]->offset;
	
		x = write(sockfd, &offset, sizeof(offset));
		if(x == -1){
			chk("writing socket");
			return -1;
		}
	
		return 0;	
	}
}



















