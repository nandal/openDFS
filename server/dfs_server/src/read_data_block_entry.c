#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_int 
read_data_block_entry(myDFS_dnode_s *dnode_ptr, myDFS_blk block_id){
	myDFS_int	fd, x;
	myDFS_char	file[MAX_PATH_SIZE];

	sprintf(file, "%s/data_block.tbl", SERVER_DATALOG);
	fd = open(file, O_RDONLY);
	if(fd == -1){
		chk("opening inode.tbl");
		return -1;
	}

	x = pread(fd, dnode_ptr, sizeof(myDFS_dnode_s), sizeof(myDFS_dhead_s) + sizeof(myDFS_dnode_s) * block_id);
	if(x == -1){
		chk("reading block table");
		return -1;
	}
	close(fd); 
	return 0;	
}
