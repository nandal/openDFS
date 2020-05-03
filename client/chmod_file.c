#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

myDFS_size chmod_file(const myDFS_char *arg){
	myDFS_char	args[MAX_PATH_SIZE];	
	myDFS_char	path[MAX_PATH_SIZE], token[MAX_PATH_SIZE];
	myDFS_int	x;
	myDFS_mode	mode;
	strcpy(args, arg);
	if(strlen(args) <= 4){			
		puts("you must specify the name of file");
	}else{
		strtok(args, " ");
		strcpy(path, strtok(NULL, " "));
		strcpy(token, strtok(NULL, " "));
		mode = atoi(token);
	}
	dfs_chmod(path, mode);
	return 0;
}
