#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

myDFS_int 
logout_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){
	myDFS_int	fd, i, x;
	myDFS_msg	msg;
	
	printf("Request to logout: %u\n", auth_ptr->uid);

	for(i = 0; i < logged_pa[auth_ptr->uid]->total_fd;){
		if(logged_pa[auth_ptr->uid]->fd_index[i] != -1){
			x = close_fd(auth_ptr, i);
			if(x == -1){
				chk("closing fd");
				msg = myDFS_MSG_FAILURE;
				x = write(sockfd, &msg, sizeof(msg));
				if(x == -1){
					chk("writing to socket");
					return -1;
				}
				return -1;
			}
			i++;
		}
	}
	free(logged_pa[auth_ptr->uid]);
	logged_pa[auth_ptr->uid] = NULL;
	printf("user session is freed.\n");
	
	comp_meta.total_logged--;
	comp_meta.free_logged++;
	

	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		return -1;
	}
	return 0;
}

