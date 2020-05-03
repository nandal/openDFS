#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../include/dfs_server.h"

myDFS_int 
creat_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	x,i, fd;
	myDFS_msg	msg;
	myDFS_CREAT_ARG	arg;
	myDFS_inode 	id;
	myDFS_inode	parent_id;
	myDFS_inode_s	inode;
	myDFS_nhead_s 	nhead;
	myDFS_nnode_s	nnode;
	myDFS_char 	file[MAX_PATH_SIZE];
	myDFS_time	t;

	time(&t);

	bzero((void*)&inode, sizeof(inode));
	inode.mode = myDFS_REG_FILE;
	inode.nlink = 1;
	inode.uid = auth_ptr->uid;
	inode.gid = auth_ptr->uid;
	inode.ctime = t;
	inode.mtime = t;
	inode.atime = t;

	x = read(sockfd, &arg, sizeof(arg));
	if(x == -1){
		chk("reading socket");
		return -1;
	}
	inode.mode = arg.mode;
	puts("args are read.");
	id = get_inode_from_path(&arg.path);
	if(id != -1){
		printf("File already exist.\n");
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
	
	id = get_new_inode_using_path(&arg.path, &inode, &parent_id);
	if(id == -1){
		printf("File inode couldnt be created.\n");
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
	lock_inode(id);
	fd = get_fd(auth_ptr, id, myDFS_O_WRONLY);

	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	x = write(sockfd, &fd, sizeof(fd));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	unlock_inode(id);
	return 0;
}




















