#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

int read_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	fd, x, i, cycles, fd_index, left, block_no;
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
	myDFS_int 	another_block=0;

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

	if(fd_global_pa[fd_index]->flags & myDFS_O_RDONLY){
		printf("flags myDFS_O_RDONLY\n");
	}else{
		puts("No read permissions");
	}
	inode = ft_global_pa[fd_global_pa[fd_index]->ftid]->inode;

	if(fd_global_pa[fd_index]->offset >= inode_global_pa[inode]->size){
		
		puts("EOF is reached.");
		return 0;	
	}
	

	block_no = fd_global_pa[fd_index]->offset / myDFS_BLKSIZ;
	block_id = inode_global_pa[inode]->data[block_no];

	if(fd_global_pa[fd_index]->offset + myDFS_BLKSIZ < inode_global_pa[inode]->size){
		another_block = 1;
	}

	read_data_block_entry(&dnode, block_id);
	get_block(&dnode);

	sprintf(file, "%s/%i/%u.txt", SERVER_WAREHOUSE, dnode.slave_id, dnode.sub_id);
	fd = open(file, O_RDONLY);
	lseek(fd, fd_global_pa[fd_index]->offset % myDFS_BLKSIZ,SEEK_SET);
	ssize = 0;
	while(x = read(fd, data, myDFS_BUFSIZ)){
		x = write(sockfd, data, x);
		ssize += x;
		if(arg.len <= ssize)break;
	}
	close(fd);
	fd_global_pa[fd_index]->offset += ssize;

	/* chek for the next block */
	if(another_block){
		block_id = inode_global_pa[inode]->data[block_no+1];
		read_data_block_entry(&dnode, block_id);
		get_block(&dnode);

		sprintf(file, "%s/%i/%u.txt", SERVER_WAREHOUSE, dnode.slave_id, dnode.sub_id);
		fd = open(file, O_RDONLY);
		ssize = 0;
		lseek(fd, fd_global_pa[fd_index]->offset % myDFS_BLKSIZ,SEEK_SET);
		while(x = read(fd, data, myDFS_BUFSIZ)){
			x = write(sockfd, data, x);
			ssize += x;
			if(arg.len <= ssize)break;
		}
		close(fd);
		fd_global_pa[fd_index]->offset += ssize;
	}

	puts("data is written.");
	return 0;	
}



















