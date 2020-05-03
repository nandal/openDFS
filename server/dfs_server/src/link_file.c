#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../include/dfs_server.h"

myDFS_int 
link_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){

	myDFS_int	x,i, fd;
	myDFS_msg	msg;
	myDFS_LINK_ARG	arg;
	myDFS_inode 	id, newid;
	myDFS_inode	dir_id;
	myDFS_char	*key;
	myDFS_inode_s	inode, newinode;
	myDFS_ihead_s	ihead;
	myDFS_path_s	path_s;
	myDFS_char 	file[MAX_PATH_SIZE]={0}, name[MAX_PATH_SIZE];
	myDFS_char	path[MAX_PATH_SIZE]={0}, dirpath[MAX_PATH_SIZE]={0};
	myDFS_time	t;

	time(&t);

	x = read(sockfd, &arg, sizeof(arg));
	if(x == -1){
		chk("reading socket");
		return -1;
	}
	puts("args are read.");
	printf("OLD : %s\n", arg.old.path);
	printf("NEW : %s\n", arg.new.path);

	id = get_inode_from_path(&arg.new);
	if(id != -1){
		printf("File already exist.\n");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		x = write(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		return -1;
	}
	/* code for creating dir entry */
	strcpy(path, arg.new.path);
	key = &path[0];

	if(path[strlen(path)-1] == '/'){
		puts("Ending with dir symbol");
		path[strlen(path)-1]='\0';
	}
	i = strlen(path);
	
	printf("len of %s is %d\n", path, i);
	do{
		printf("key[%i] = %c\n", i-1, key[i-1]);
		i--;
	}while(key[i] != '/');
	strcpy(name, path+i+1);
	strncpy(dirpath, path, i);
	printf("index of / is %d\n", i);
	printf("Name of file %s\n", name);
	printf("Directory path %s\n", dirpath);

	create_path(&path_s, dirpath);
	dir_id = get_inode_from_path(&path_s);
	/////////////////////////////////////

	id = get_inode_from_path(&arg.old);
	if(newid == -1){
		printf("File doesnt exist.\n");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		x = write(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		return -1;
	}
	make_dir_entry(name, dir_id, id);

	read_inode_entry(id, &inode);
	inode.nlink++;
	update_inode_entry(&inode);

	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	x = write(sockfd, &id, sizeof(id));
	if(x == -1){
		chk("writing socket");
		return -1;
	}
	return 0;	
}














