#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

myDFS_size read_file(const myDFS_char *arg){
	
	myDFS_char 	args[MAX_PATH_SIZE];
	myDFS_char	filepath[MAX_PATH_SIZE];
	myDFS_char	data[myDFS_BUFSIZ]={0};
	myDFS_int	fd, x;
	strcpy(args, arg);
	if(strlen(args) <= 4){			
		puts("you must specify the name of file");
	}else{
		strtok(args, " ");
		strcpy(filepath, strtok(NULL, " "));
	}

	fd = dfs_open(filepath, myDFS_O_RDONLY, 0);
	printf("FD of given file %d\n", fd);
	puts("-----------------CONTENTS OF FILE------------------\n");
	
	while(x = dfs_read(fd, data, myDFS_BUFSIZ)){
		if(x == -1){
			chk("reading from fd");
			dfs_close(fd);
			return -1;
		}
		x = write(1, data, x);
		bzero(data, myDFS_BUFSIZ);
	}
	dfs_close(fd);
	return 0;
}
