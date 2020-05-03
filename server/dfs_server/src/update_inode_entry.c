#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_int 
update_inode_entry(myDFS_inode_s *inode_ptr){
	myDFS_int	fd, x;
	myDFS_offset	position;
	myDFS_char	file[MAX_PATH_SIZE];

	sprintf(file, "%s/inode.tbl", SERVER_DATALOG);
	fd = open(file, O_WRONLY);
	if(fd == -1){
		chk("opening inode table");
		return -1;
	}
	position = sizeof(myDFS_ihead_s) + sizeof(myDFS_inode_s) * inode_ptr->id;
	x = pwrite(fd, inode_ptr, sizeof(myDFS_inode_s), position);
	if(x == -1){
		chk("inode entry in inode table");
		return -1;
	}
	return 0;
}
