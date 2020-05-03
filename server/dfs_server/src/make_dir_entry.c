#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_int 
make_dir_entry(const myDFS_char *fname, myDFS_inode dir_id, myDFS_inode file_id){
	myDFS_int	fd, fdir, i, x;
	myDFS_nhead_s 	nhead;
	myDFS_nnode_s 	nnode;
	myDFS_char	file[MAX_FILE_NAME];
	myDFS_inode_s	dir_inode;
	myDFS_time 	t;

	sprintf(file, "%s/dir/%u.tbl", SERVER_DATALOG, dir_id);

	fd = open(file, O_RDWR);
	if(fd == -1){
		printf("Could not open dir.tbl\n");
		return -1;
	}
	puts(fname);
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek the start of dir");
		close(fd);
		return -1;
	}

	x = read(fd, &nhead, sizeof(nhead));
	if(x == -1){
		chk("reading header of dir");
		close(fd);
		return -1;
	}
	if(nhead.free == 0){
		printf("Directory is full.\n");
		printf("No more file can be added.\n");
		return -1;
	}



	/* make dir entry */
	do{
		x = read(fd, &nnode, sizeof(nnode));
		if(x == -1){
			chk("reading name in dir");
			close(fd);
			return -1;
		}
	}while(strlen(nnode.name) != 0);

	strcpy(nnode.name, fname);
	nnode.inode = file_id;
	printf("nnode.name : %s nnode.inode : %u\n", nnode.name, nnode.inode);
	x = lseek(fd, -sizeof(nnode), SEEK_CUR);
	if(x == -1){
		chk("lseek the name in dir");
		close(fd);
		return -1;
	}
	x = write(fd, &nnode, sizeof(nnode));
	if(x == -1){
		chk("writing name in dir");
		close(fd);
		return -1;
	}

	nhead.free--;
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek the head of dir");
		close(fd);
		return -1;
	}
	x = write(fd, &nhead, sizeof(nhead));
	if(x == -1){
		chk("writing header in dir");
		close(fd);
		return -1;
	}
	close(fd);
	
	/* update directories inode entry */
	sprintf(file, "%s/inode.tbl", SERVER_DATALOG);
	fdir = open(file, O_RDWR);
	if(fdir == -1){
		chk("opening inode table");
		return -1;
	}
	x = lseek(fdir, sizeof(myDFS_ihead_s) + sizeof(myDFS_inode_s) * dir_id, SEEK_SET);
	if(x == -1){
		chk("lseek the inode entry in inode table");
		close(fdir);
		return -1;
	}
	x = read(fdir, &dir_inode, sizeof(myDFS_inode_s));
	if(x == -1){
		chk("reading the inode of dir");
		close(fdir);
		return -1;
	}
	dir_inode.blocks++;
	time(&t);
	dir_inode.atime = t;
	dir_inode.mtime = t;
	x = lseek(fdir, -sizeof(myDFS_inode_s), SEEK_CUR);
	if(x == -1){
		chk("lseek the inode entry of dir");
		close(fdir);
		return -1;
	}
	x = write(fdir, &dir_inode, sizeof(myDFS_inode_s));
	if(x == -1){
		chk("writing inode of dir");
		close(fdir);
		return -1;
	}
	close(fdir);
	return 0;	
}
