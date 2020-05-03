#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/dfs_client.h"


int main(int argc, char **argv){
	short u;
	u = atoi(argv[1]);
	printf("MODE : %o\n", u);
	if(myDFS_IS_DIR(u))printf("DIRECTORY\n");
	if(myDFS_IS_REG(u))printf("REGULAR\n");
	return 0;
}
