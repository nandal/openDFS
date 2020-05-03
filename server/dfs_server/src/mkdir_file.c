#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../include/dfs_server.h"

myDFS_int 
mkdir_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

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
	inode.mode = myDFS_DIR_FILE;
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
	
	sprintf(file, "%s/dir/%u.tbl", SERVER_DATALOG, id);
	fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("creating dir id.tbl");
	}
	nhead.free = MAX_DIR_ENTRY;
	nhead.self = id;
	nhead.parent = parent_id;
	bzero(nnode.name, MAX_FILE_NAME);
	nnode.inode = 0;
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek of dir id.tbl");
		return -1;
	}
	x = write(fd, &nhead, sizeof(myDFS_nhead_s));
	if(x == -1){
		chk("writing to dir id.tbl");
		return -1;
	}
	for(i = 0; i < MAX_DIR_ENTRY; i++){
		x = write(fd, &nnode, sizeof(myDFS_nnode_s));
		if(x == -1){
			chk("writing to dir file");
			return -1;
		}
	}
	x = close(fd);
	if(x == -1){
		chk("closing dir id.tbl");
	}
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




















