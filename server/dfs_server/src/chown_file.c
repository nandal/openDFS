#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../include/dfs_server.h"

myDFS_int 
chown_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	
	myDFS_int	x;
	myDFS_msg	msg;
	myDFS_CHOWN_ARG	arg;
	myDFS_inode	id;
	myDFS_inode_s	inode;
	myDFS_time	t;

	time(&t);

	x = read(sockfd, &arg, sizeof(arg));
	if(x == -1){
		chk("reading socket");
		return -1;
	}
	puts("args are read.");
	id = get_inode_from_path(&arg.path);
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

	read_inode_entry(id, &inode);
	inode.uid = arg.uid;
	inode.gid = arg.gid;
	update_inode_entry(&inode);
	
	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	x = write(sockfd, &id, sizeof(id));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	return 0;	
}
