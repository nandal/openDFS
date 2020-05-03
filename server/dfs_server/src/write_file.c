#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

int write_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	fd, x, i, cycles, fd_index, left;
	myDFS_msg	msg;
	myDFS_RW_ARG	arg;
	myDFS_inode 	inode;
	myDFS_dnode_s	dnode;
	myDFS_ssize	ssize = 0;
	myDFS_char	file[MAX_PATH_SIZE], data[myDFS_BUFSIZ];
	myDFS_blk	data_node_id;
	myDFS_dev	slave_id;
	myDFS_ssize	extra_space, extra_data;
	myDFS_int 	new_blocks;
	myDFS_blk	block_id;

	x = read(sockfd, &arg, sizeof(arg));
	if(x == -1){
		chk("reading socket");
		return -1;
	}
	puts("args are read.");
	

	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}

	fd_index = logged_pa[auth_ptr->uid]->fd_index[arg.fd];
	printf("fd %d, len %d\n", arg.fd, arg.len);
	printf("offset %u\n", fd_global_pa[fd_index]->offset);

	if(fd_global_pa[fd_index]->flags & myDFS_O_WRONLY){
		printf("flags myDFS_O_WRONLY\n");
	}else{
		puts("No write permissions");
	}
	inode = ft_global_pa[fd_global_pa[fd_index]->ftid]->inode;
	extra_data = (fd_global_pa[fd_index]->offset + arg.len) - inode_global_pa[inode]->size;
	extra_space = (fd_global_pa[fd_index]->offset + arg.len) - inode_global_pa[inode]->blocks * myDFS_BLKSIZ;
	printf("Extra Space needed : %d\n", extra_space);

	if(extra_space > 0){
		puts("One more data block is needed");
		block_id = get_free_data_block(&dnode);
		inode_global_pa[inode]->data[inode_global_pa[inode]->blocks] = block_id;
		inode_global_pa[inode]->blocks++;
		sprintf(file, "%s/%i/%u.txt", SERVER_WAREHOUSE, dnode.slave_id, dnode.sub_id);
		fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if(fd == -1){
			chk("creationg of dnode file");
			return -1;
		}

		cycles = arg.len / myDFS_BUFSIZ;
		left = arg.len % myDFS_BUFSIZ;
		for(i = 0; i < cycles; i++){
			x = read(sockfd, data, myDFS_BUFSIZ);
			x = write(fd, data, x);
			x = write(1, data, x);
			ssize += x;
		}
		x = read(sockfd, data, left);
		x = write(fd, data, x);
		x = write(1, data, x);
		ssize += x;
		close(fd);
	}
	write_block(&dnode);
	
	fd_global_pa[fd_index]->offset += ssize;

	if(fd_global_pa[fd_index]->offset > inode_global_pa[inode]->size){
		inode_global_pa[inode]->size = fd_global_pa[fd_index]->offset;
	}

	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	puts("result msg is written.");
	x = write(sockfd, &ssize, sizeof(ssize));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	puts("ssize is written.");
	return 0;	
}



















