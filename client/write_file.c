#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

myDFS_size write_file(const myDFS_char *arg){
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
	fdin = open(src, O_RDONLY);
	fdout = dfs_creat(targ, 0666);
	printf("FD OUT of targ file : %d\n", fdout);
	printf("FD IN of src file : %d\n", fdin);
	while(x = read(fdin, data, myDFS_BLKSIZ)){
		if(x == -1){
			chk("reading from fdin");
			close(fdin);
			dfs_close(fdout);
			return -1;
		}
		x = dfs_write(fdout, data, x);
		x = write(1, data, x);
	}
	close(fdin);
	dfs_close(fdout);
	return 0;
}
