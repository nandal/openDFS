#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_int 
read_inode_entry(myDFS_inode inode_id, myDFS_inode_s *inode_ptr){
	myDFS_int	fd, x;
	myDFS_ihead_s 	ihead;
	myDFS_time	t;
	myDFS_char	file[MAX_PATH_SIZE];

	sprintf(file, "%s/inode.tbl", SERVER_DATALOG);
	fd = open(file, O_RDWR);
	if(fd == -1){
		chk("opening inode.tbl");
		return -1;
	}

	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseeking the inode table");
		return -1;
	}
	x = read(fd, &ihead, sizeof(ihead));
	if(x == -1){
		chk("reading head of inode table");
		return -1;
	}
	if(ihead.free == MAX_NO_FILES){
		printf("File System is empty.\n");
		return -1;
	}
	
	x = lseek(fd, sizeof(myDFS_inode_s) * inode_id, SEEK_CUR);
	if(x == -1){
		chk("lseeking the inode table");
		return -1;
	}
	x = read(fd, inode_ptr, sizeof(myDFS_inode_s));
	if(x == -1){
		chk("reading inode entry");
		return -1;
	}

	x = lseek(fd, -sizeof(myDFS_inode_s), SEEK_CUR);
	if(x == -1){
		chk("lseeking the inode table");
		return -1;
	}

	/* update access time */
	time(&t);
	inode_ptr->atime = t;
	x = write(fd, inode_ptr, sizeof(myDFS_inode_s));
	if(x == -1){
		chk("writing the update time inode entry");
		return -1;
	}


	close(fd); 
	return 0;	
}
