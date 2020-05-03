#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

int make_dir(const myDFS_char *arg){
	myDFS_char 	args[MAX_PATH_SIZE];
	myDFS_char	filepath[MAX_PATH_SIZE];
	strcpy(args, arg);
	if(strlen(args) <= 4){			
		puts("you must specify the name of file");
	}else{
		strtok(args, " ");
		strcpy(filepath, strtok(NULL, " "));
	}
	printf("FD of new dir %d\n",dfs_mkdir(filepath, 0777));
	return 0;
}
