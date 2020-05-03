#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

int change_dir(const myDFS_char *arg){
	myDFS_char	args[50];
	myDFS_char	token[50];
	int i;
	strcpy(args, arg);
	strcpy(token, strtok(args, " "));
	printf("%s\n",token);
	strcpy(token, strtok(NULL, " "));
	printf("%s\n",token);
	dfs_chdir(token);
	if(strcmp(token, ".")==0)
		printf("current directory\n");
	else if(strcmp(token, "..")==0)
		printf("parent directory\n");
	else 
		printf("Target dir %s\n", token);
	return 0;
}
