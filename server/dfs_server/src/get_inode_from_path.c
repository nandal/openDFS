#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_inode get_inode_from_path(const myDFS_path_s *path_ptr){
	myDFS_int	i, tokens;
	myDFS_char	name[MAX_FILE_NAME];
	myDFS_inode	dir_id = 0;
	myDFS_char	path[MAX_PATH_SIZE] = "";
	myDFS_int	depth = 0;
	
	sprintf(path, "%s", path_ptr->path);
	tokens = path_ptr->depth;
	if(path_ptr->path[strlen(path_ptr->path)-1] != '/')tokens++;
	if(tokens == 1){
		printf("root dir");
		return 0;
	}else{
		strcpy(name, strtok(path, "/"));
		if(tokens == 2){
			return name_to_inode(name, 0);
		}
		dir_id = name_to_inode(name, 0);
		if(dir_id == -1){
			chk("geting dir id from path");
			return -1;
		}
		for(i = 2; i < tokens; i++){
			strcpy(name, strtok(NULL, "/"));
			dir_id = name_to_inode(name, dir_id);
			if(dir_id == -1){
				chk("geting dir id from path");
				return -1;
			}
		}
		return dir_id;		
	}
}
/*
int main(int argc, char *argv[]){
	myDFS_path_s path;
	strcpy(path.path, argv[1]);
	create_path(&path, argv[1]);
	printf("inode %i\n",get_inode_from_path(&path));
	return 0;
	
}*/
