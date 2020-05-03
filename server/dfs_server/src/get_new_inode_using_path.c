#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dfs_server.h"

myDFS_inode 
get_new_inode_using_path(const myDFS_path_s *path_ptr, myDFS_inode_s *inode_ptr, myDFS_inode *parent_ptr){
	myDFS_int	i, x,tokens;
	myDFS_char	name[MAX_FILE_NAME];
	myDFS_inode	dir_id = 0;
	myDFS_char	path[MAX_PATH_SIZE] = "";
	myDFS_int	depth = 0;
	myDFS_inode_s	dir_node;
	myDFS_inode	file_id = 0;

	bzero((void *)&dir_node, sizeof(dir_node));
	
	sprintf(path, "%s", path_ptr->path);
	tokens = path_ptr->depth;
	if(path_ptr->path[strlen(path_ptr->path)-1] != '/')tokens++;
	if(tokens == 1){
		return file_id;
	}else{
		dir_id = 0;
		strcpy(name, strtok(path, "/"));
		if(tokens == 2){
			file_id = make_inode_entry(inode_ptr);
			if(file_id == -1){
				puts("make_inode_entry failed.\n");
				return -1;
			}
			x = make_dir_entry(name, dir_id, file_id);
			if(x == -1){
				puts("directory entry creation failed.");
				return -1;
			}
			*parent_ptr = 0;
			return file_id;
		}
		dir_id = name_to_inode(name, dir_id);
		if(dir_id == -1){
			chk("File not exists of path");
			return -1;
		}
		x = read_inode_entry(dir_id, &dir_node);
		if(x == -1){
			chk("Reading of Inode Entry");
			return -1;
		}
		if(!myDFS_IS_DIR(dir_node.mode)){
			chk("Path contains a Regular file in between.");
			return -1;
		}


		for(i = 2; i < tokens-1; i++){
			strcpy(name, strtok(NULL, "/"));
			dir_id = name_to_inode(name, dir_id);
			if(dir_id == -1){
				chk("File not exists.");
				return -1;
			}
			x = read_inode_entry(dir_id, &dir_node);
			if(x == -1){
				chk("Reading of Inode Entry");
				return -1;
			}
			if(!myDFS_IS_DIR(dir_node.mode)){
				chk("Path contains a Regular file in between.");
				return -1;
			}
			
		}
		strcpy(name, strtok(NULL, "/"));
		file_id = make_inode_entry(inode_ptr);
		if(file_id == -1){
			chk("Failed making inode entry");
			return -1;
		}
		x = make_dir_entry(name, dir_id, file_id);
		if(x == -1){
			chk("Failed making directory entry");
			return -1;
		}
		*parent_ptr = dir_id;
		return file_id;		
	}
}
/*
int main(int argc, char *argv[]){
	myDFS_path_s path;
	strcpy(path.path, argv[1]);
	create_path(&path, argv[1]);
	printf("inode %i\n",get_new_inode_using_path(&path));
	return 0;
	
}*/
