#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

myDFS_size link_file(const myDFS_char *arg){
	myDFS_char	args[MAX_PATH_SIZE];	
	myDFS_char	src[MAX_PATH_SIZE], targ[MAX_PATH_SIZE];
	myDFS_int	fdin, fdout, x;
	myDFS_char	data[myDFS_BLKSIZ];
	strcpy(args, arg);
	if(strlen(args) <= 4){			
		puts("you must specify the name of file");
	}else{
		strtok(args, " ");
		strcpy(src, strtok(NULL, " "));
		strcpy(targ, strtok(NULL, " "));
	}
	dfs_link(src, targ);
	return 0;
}
