#include <stdio.h>
#include <stdlib.h>
#include "../include/dfs_server.h"

int process_request(myDFS_int sockfd, const myDFS_msg chkmsg){

	myDFS_int 	x;
	myDFS_msg	msg;
	myDFS_auth_s	auth;

	msg = myDFS_MSG_FAILURE;

	printf("myDFS_msg = %d\n", chkmsg);

	/* reading accessing details of user */

	x = read(sockfd, &auth, sizeof(auth));
	if(x == -1){
		msg = myDFS_MSG_FAILURE;
		chk("reading authorization from socket");
		write(sockfd, &msg, sizeof(msg));
		return -1;
	}
	x = access_permission(&auth, msg);
	if(x != 0){
		printf("Access Denied\n");
		msg = myDFS_MSG_FAILURE;
		write(sockfd, &msg, sizeof(msg));
		return -1;
	}

	/* PROCESS REQUEST */
	switch(chkmsg){

	/* OPEN REQUEST */
	case myDFS_MSG_OPEN:{
		puts("request of open file function.");
		x = open_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* CLOSE REQUEST */
	case myDFS_MSG_CLOSE:{
		puts("request to close file discriptor.");
		x = close_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* READ REQUEST */
	case myDFS_MSG_READ:{
		x = read_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;


	/* WRITE REQUEST */
	case myDFS_MSG_WRITE:{
		x = write_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;


	/* LSEEK REQUEST */
	case myDFS_MSG_LSEEK:{
		x = lseek_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* CREAT REQUEST */
	case myDFS_MSG_CREAT:{
		x = creat_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* CHDIR REQUEST */
	case myDFS_MSG_CHDIR:{
		x = chdir_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;


	/* GETCWD REQUEST */
	case myDFS_MSG_GETCWD:{
		x = getcwd_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* CHMOD REQUEST */
	case myDFS_MSG_CHMOD:{
		x = chmod_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* CHOWN REQUEST */
	case myDFS_MSG_CHOWN:{
		x = chown_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* LINK REQUEST */
	case myDFS_MSG_LINK:{
		x = link_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* MKDIR REQUEST */
	case myDFS_MSG_MKDIR:{
		x = mkdir_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* RMDIR REQUEST */
	case myDFS_MSG_RMDIR:{
		x = rmdir_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* UNLINK REQUEST */
	case myDFS_MSG_UNLINK:{
		x = unlink_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* STOP REQUEST */
	case myDFS_MSG_STOP:{
//		x = stop_slaves(&auth);
		if(x == -1){
			printf("An error occured.\n");
			perror("process_request.c:52");
			msg = myDFS_MSG_FAILURE;
			x = write(sockfd, &msg, sizeof(msg));
			if(x == -1){
				chk("writing to socket");
				return -1;
			}
		}
		msg = myDFS_MSG_SUCCESS;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		close(sockfd);
		exit(0);
		}
		break;

	/* FORMAT REQUEST */
	case myDFS_MSG_FORMAT:{
		msg = myDFS_MSG_SUCCESS;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
//		x = format(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* LOGOUT REQUEST */
	case myDFS_MSG_LOGOUT:{
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		x = logout_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;

	/* STATUS REQUEST */
	case myDFS_MSG_STATUS:{
		printf("Request for showing status\n");
		if(x == -1){
			chk("writing to socket");
			return -1;
		}
		x = status_file(sockfd, &auth);
		if(x == -1){
			chk("reading of file failed");
			}
		}
		break;


	}
	return 0;
}
