#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../include/dfs_server.h"

myDFS_int 
chdir_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	x;
	myDFS_msg	msg;
	myDFS_int 	ret = 0;
	myDFS_inode	id;
	myDFS_path_s	path;

	
	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		return -1;
	}

	x = read(sockfd, &path, sizeof(path));
	if(x == -1){
		chk("reading socket");
		return -1;
	}

	id = get_inode_from_path(&path);
	if(id == -1){
		printf("File doesnt exist.\n");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		x = write(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		return -1;
	}

	logged_pa[auth_ptr->uid]->current = path;
	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	
	x = write(sockfd, &ret, sizeof(ret));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	return 0;	
}




















