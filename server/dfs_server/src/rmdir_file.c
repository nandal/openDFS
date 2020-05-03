#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../include/dfs_server.h"

myDFS_int 
rmdir_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	x,i, fd;
	myDFS_msg	msg;
	myDFS_CREAT_ARG	arg;
	myDFS_inode 	id;
	myDFS_inode	parent_id;
	myDFS_inode_s	inode;
	myDFS_ihead_s	ihead;
	myDFS_nhead_s 	nhead;
	myDFS_nnode_s	nnode;
	myDFS_char 	file[MAX_PATH_SIZE];
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
		printf("Dir doent exist.\n");
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
	fd = open(file, O_RDONLY);
	x = read(fd, &nhead, sizeof(nhead));
	parent_id = nhead.parent;
	close(fd);
	if(nhead.free == MAX_DIR_ENTRY){
		unlink(file);
		read_inode_entry(id, &inode);
		inode.nlink--;
		if(inode.nlink == 0){
			sprintf(file, "%s/inode.tbl", SERVER_DATALOG);
			fd = open(file, O_RDWR);
			x = read(fd, &ihead, sizeof(ihead));
			ihead.free++;
			x = lseek(fd, 0, SEEK_SET);
			x = write(fd, &ihead, sizeof(ihead));
			close(fd);
		}		
		update_inode_entry(&inode);

		delete_dir_entry(arg.path.path);

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
	}else{
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing socket");
			return -1;
		}
		x = write(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		return -1;

	}
}


















