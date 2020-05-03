#include <stdio.h>
#include <string.h>
#include "../include/dfs_server.h"

int count_char(const myDFS_char *str, const myDFS_char ch);

int create_path(myDFS_path_s *path_ptr, const myDFS_char *path){
	int i;
	strcpy(path_ptr->path, path);
	if(strncmp(path_ptr->path, "/" , 1) == 0){
		path_ptr->type = myDFS_FULL_PATH;
	}else{
		path_ptr->type = myDFS_REL_PATH;
	}
	path_ptr->depth = count_char(path_ptr->path, '/');

	return 0;
}
int count_char(const myDFS_char *str, const myDFS_char ch){
	int n = 0;
	while(*str){
		if(*(str)++ == '/')n++;
	}
	return n;
}
