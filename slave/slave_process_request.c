#include <stdio.h>
#include <stdlib.h>
#include "header/slave.h"

int slave_process_request(myDFS_int sockfd, const myDFS_msg msg_ptr){
	myDFS_int	x = 0;
	myDFS_msg	msg;

	msg = myDFS_MSG_FAILURE;
	printf("myDFS_msg %d\n", msg);

	switch(msg_ptr){
	case myDFS_MSG_READ:{
		perror("process_request:12:read");
		slave_read_data(sockfd);
		msg = myDFS_MSG_SUCCESS;
		}
		break;
	case myDFS_MSG_WRITE:{	
		slave_write_data(sockfd);
		msg = myDFS_MSG_SUCCESS;
		write(sockfd, &msg, sizeof(myDFS_msg));
		}
		break;
	case myDFS_MSG_STOP:{
		printf("Asked to stop by server");
		msg = myDFS_MSG_SUCCESS;
		write(sockfd, &msg, sizeof(myDFS_msg));
		exit(0);
		}
		break;
	case myDFS_MSG_FORMAT:{
		printf("Server asked to format the system.\n");
		msg = myDFS_MSG_SUCCESS;
		}
		break;
	}
	return 0;
}
