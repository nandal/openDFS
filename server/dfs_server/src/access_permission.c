#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/dfs_server.h"

int access_permission(const myDFS_auth_s *auth_ptr, const myDFS_msg msg){

	myDFS_usnode_s	*usnode_p;
	usnode_p = logged_pa[auth_ptr->uid];

	if(usnode_p->key == auth_ptr->key){
		return 0;
	}else{
		return -1;
	}

}
