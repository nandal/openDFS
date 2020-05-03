#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/dfs_server.h"

myDFS_int 
free_data_block(const myDFS_blk data_id){
	myDFS_int 	fd, x;
	myDFS_dhead_s	dhead;
	myDFS_dnode_s	dnode;
	myDFS_char	file[MAX_PATH_SIZE];

	/* free data blocks holded by inode */
	sprintf(file, "%s/data_block.tbl", SERVER_DATALOG);
	fd = open(file, O_RDWR);
	if(fd == -1){
		chk("opening data_blocks table");
		return -1;
	}

	x = read(fd, &dhead, sizeof(dhead));
	if(x == -1){
		chk("readin header of data block");
		close(fd);
		return -1;
	}
	
	if(dhead.free == TOTAL_BLOCKS){
		printf("All blocks are free. %i\n", dhead.free);
		close(fd);
		return -1;
	}

	x = pread(fd, &dnode, sizeof(dnode), sizeof(dhead) + sizeof(dnode) * data_id);
	if(x == -1){
		chk("reading data node of data block table");
		close(fd);
		return -1;
	}
	dhead.free++;
	dnode.status = AVAILABLE;

	x = pwrite(fd, &dnode, sizeof(dnode), sizeof(dhead) + sizeof(dnode) * data_id);
	if(x == -1){
		chk("writing data node of data table");
		close(fd);
		return -1;
	}

	x = pwrite(fd, &dhead, sizeof(dhead), 0);
	if(x == -1){
		chk("writing head of data block table");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}
