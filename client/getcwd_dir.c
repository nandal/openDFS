#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "include/client.h"

int getcwd_dir(){
	myDFS_path_s	path={0};
	printf("%s\n", dfs_getcwd(path.path, sizeof(path.path)));
	return 0;
}
