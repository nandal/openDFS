#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../include/dfs_server.h"

myDFS_int 
getcwd_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){
	myDFS_int	x;
	myDFS_path_s	path;
	myDFS_msg	msg;

	path = logged_pa[auth_ptr->uid]->current;
	
	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		return -1;
	}
	x = write(sockfd, &path, sizeof(path));
	printf("path : %s\n", path.path);
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	return 0;	
}




















