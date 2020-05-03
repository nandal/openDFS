#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_int delete_dir_entry(const myDFS_char *path){
	myDFS_char 	name[MAX_FILE_NAME], path_cpy[MAX_PATH_SIZE];
	myDFS_char	*key;
	myDFS_char	file[MAX_PATH_SIZE];
	myDFS_nnode_s	nnode;
	myDFS_nhead_s	nhead;
	myDFS_inode_s	inode;
	myDFS_path_s	path_s={0};
	myDFS_int 	i, x, fd;
	myDFS_inode 	dir_id;
	myDFS_char	dirpath[MAX_PATH_SIZE] = {0};

	strcpy(path_cpy, path);
	key = &path_cpy[0];

	if(path_cpy[strlen(path_cpy)-1] == '/'){
		puts("Ending with dir symbol");
		path_cpy[strlen(path_cpy)-1]='\0';
	}
	i = strlen(path_cpy);
	
	printf("len of %s is %d\n", path_cpy, i);
	do{
		printf("key[%i] = %c\n", i-1, key[i-1]);
		i--;
	}while(key[i] != '/');
	strcpy(name, path_cpy+i+1);
	strncpy(dirpath, path_cpy, i);
	printf("index of / is %d\n", i);
	printf("Name of file %s\n", name);
	printf("Directory path %s\n", dirpath);

	create_path(&path_s, dirpath);
	dir_id = get_inode_from_path(&path_s);
	sprintf(file, "%s/dir/%u.tbl", SERVER_DATALOG, dir_id);

	fd = open(file, O_RDWR);
	if(fd == -1){
		printf("Could not open dir.tbl\n");
		return -1;
	}
	puts(name);
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


	/* remove dir entry */
	do{
		x = read(fd, &nnode, sizeof(nnode));
		if(x == -1){
			chk("reading name in dir");
			close(fd);
			return -1;
		}
	}while(strcmp(nnode.name, name) != 0);

	bzero(nnode.name, strlen(nnode.name));
	lseek(fd, -sizeof(nnode), SEEK_CUR);
	x = write(fd, &nnode, sizeof(nnode));
	nhead.free++;
	x = pwrite(fd, &nhead, sizeof(nhead), 0);

	read_inode_entry(dir_id, &inode);
	inode.blocks--;
	update_inode_entry(&inode);

	close(fd);
	return 0;
}

