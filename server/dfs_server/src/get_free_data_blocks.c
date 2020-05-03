#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/dfs_server.h"

myDFS_int 
get_free_data_blocks(myDFS_blk *data, myDFS_blk n){
	myDFS_int 	fd, x;
	myDFS_blk	i, j;
	myDFS_dhead_s	dhead;
	myDFS_dnode_s	dnode;
	myDFS_char	file[MAX_PATH_SIZE];

	/* get free data blocks to store the file */
	sprintf(file, "%s/data_block.tbl", SERVER_DATALOG);
	fd = open(file, O_RDWR);
	if(fd == -1){
		chk("opening data blocks table");
		return -1;
	}

	x = read(fd, &dhead, sizeof(myDFS_dhead_s));
	if(x == -1){
		chk("reading header of data blocks");
		close(fd);
		return -1;
	}
	
	if(dhead.free < n){
		printf("Not enough space. %i\n", dhead.free);
		close(fd);
		return -1;
	}
	for(i = 0, j = 0; j < n; i++){
		x = read(fd, &dnode, sizeof(myDFS_dnode_s));
		if(x == -1){
			chk("reading data node from data block table");
			close(fd);
			return -1;
		}

		if(dnode.status == AVAILABLE){
			dhead.free--;
			dnode.status = OCCUPIED;
			data[j++] = dnode.id;
			x = lseek(fd, - sizeof(myDFS_dnode_s), SEEK_CUR);
			if(x == -1){
				chk("lseek data node from data block table");
				close(fd);
				return -1;
			}
			x = write(fd, &dnode, sizeof(myDFS_dnode_s));
			if(x == -1){
				chk("writing data node to data block table");
				close(fd);
				return -1;
			}
		}

	}
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek head data block table");
		close(fd);
		return -1;
	}
	x = write(fd, &dhead, sizeof(myDFS_dhead_s));
	if(x == -1){
		chk("writing head node to data block table");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}
