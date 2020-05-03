#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_inode 
make_inode_entry(myDFS_inode_s *inode_ptr){
	myDFS_int	fd, x;
	myDFS_ihead_s 	ihead;
	myDFS_inode_s	inode;
	myDFS_time	t;
	myDFS_char	file[MAX_PATH_SIZE];

	sprintf(file, "%s/inode.tbl", SERVER_DATALOG);
	fd = open(file, O_RDWR);
	if(fd == -1){
		chk("opening inode table");
		return -1;
	}

	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek the inode table");
		return -1;
	}
	x = read(fd, &ihead, sizeof(ihead));
	if(x == -1){
		chk("reading ihead from inode table");
		close(fd);
		return -1;
	}
	if(ihead.free == 0){
		printf("File System is full.\n");
		printf("No more file can be added.\n");
		return -1;
	}
	

	do{
		x = read(fd, &inode, sizeof(inode));
		if(x == -1){
			chk("reading inode");puts("loop inode");
			close(fd);
			return -1;
		}
	}while(inode.nlink > 0);
	printf("inode.id %u\n", inode.id);

	inode_ptr->id = inode.id;
	x = lseek(fd, - sizeof(inode), SEEK_CUR);
	if(x == -1){
		chk("lseek inode table");
		close(fd);
		return -1;
	}
	x = write(fd, inode_ptr, sizeof(inode));
	if(x == -1){
		chk("writing update inode to inode table");
		close(fd);
		return -1;
	}

	ihead.free--;
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek inode table");
		close(fd);
		return -1;
	}
	x = write(fd, &ihead, sizeof(ihead));
	if(x == -1){
		chk("writing update ihead to inode table");
		close(fd);
		return -1;
	}
	close(fd);

	return inode_ptr->id;	
}
