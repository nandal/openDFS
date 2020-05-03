#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_inode name_to_inode(const myDFS_char *fname, myDFS_inode dir_id){
	myDFS_int 	fd, i, x;
	myDFS_nhead_s 	nhead;
	myDFS_nnode_s 	nnode;
	myDFS_char	dirfile[MAX_FILE_NAME];

	sprintf(dirfile, "%s/dir/%u.tbl", SERVER_DATALOG, dir_id);
	fd = open(dirfile, O_RDONLY);
	if(fd == -1){
		chk("opening dir");
		return -1;
	}

	x = read(fd, &nhead, sizeof(myDFS_nhead_s));
	if(x == -1){
		chk("reading dir header");
		close(fd);
		return -1;
	}
	if(strcmp(fname, ".") == 0)return nhead.self;
	if(strcmp(fname, "..") == 0)return nhead.parent;

	if(nhead.free == MAX_DIR_ENTRY){
		printf("Dir is empty.\n");
		return -1;
	}

	for(i = 0; i < MAX_DIR_ENTRY; i++){
		x = read(fd, &nnode, sizeof(myDFS_nnode_s));
		if(x == -1){
			chk("reading the name in dir");
			close(fd);
			return -1;
		}
		if(strcmp(nnode.name, fname) == 0){
			close(fd);
			return nnode.inode;
		}
	}
	close(fd);
	return -1;	
}
