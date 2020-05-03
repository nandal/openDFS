#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

myDFS_size chown_file(const myDFS_char *arg){
	myDFS_char	args[MAX_PATH_SIZE];	
	myDFS_char	path[MAX_PATH_SIZE], token[MAX_PATH_SIZE];
	myDFS_int	x, uid, gid;
	strcpy(args, arg);
	if(strlen(args) <= 4){			
		puts("you must specify the name of file");
	}else{
		strtok(args, " ");
		strcpy(path, strtok(NULL, " "));
		strcpy(token, strtok(NULL, " "));
		uid = atoi(token);
		strcpy(token, strtok(NULL, " "));
		gid = atoi(token);
	}
	dfs_chown(path, uid, gid);
	return 0;
}
