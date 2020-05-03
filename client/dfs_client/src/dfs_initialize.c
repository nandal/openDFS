#include <stdio.h>
#include "../include/dfs_client.h"


myDFS_path_s	current_dir;
myDFS_char	user[MAX_USERNAME] = "";
myDFS_auth_s	auth;	/* an authorization certificate recieved on successful login */
myDFS_char	server_ip[MAX_IP] = SERVER_IP;
myDFS_short	server_port = SERVER_PORT;
myDFS_ERROR	ERROR = 0;


int dfs_initialize(){
	puts("all global variables are initialized.");
	return 0;
}
