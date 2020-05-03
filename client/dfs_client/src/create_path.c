#include <stdio.h>
#include <string.h>
#include "../include/dfs_client.h"

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
/*
int main(int argc, char *argv[]){
	myDFS_path_s 	cur;
	strcpy(cur.path, argv[1]);
	create_path(&cur, cur.path);
	printf("Path : %s\n", cur.path);
	printf("Type : %d\n", cur.type);
	printf("Depth : %d\n", cur.depth);
	return 0;
}
*/
