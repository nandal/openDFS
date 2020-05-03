#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../include/dfs_client.h"

myDFS_int
dfs_chdir(const myDFS_char *path){
	myDFS_int	sockfd;
	myDFS_int	x, ret;
	myDFS_msg	msg;
	myDFS_path_s	path_s;
	struct sockaddr_in address;


	msg = myDFS_MSG_CHDIR;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		chk("socket creation");
		return -1;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(server_ip);
	address.sin_port = htons(server_port);

	x = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if(x == -1){
		chk("connect");
		ERROR = myDFS_ESCONNECT;
		close(sockfd);
		return -1;
	}
	
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		ERROR = myDFS_ESWRITE;
		close(sockfd);
		return -1;
	}

	x = write(sockfd, &auth, sizeof(auth));
	if(x == -1){
		chk("writing to socket");
		ERROR = myDFS_ESWRITE;
		close(sockfd);
		return -1;
	}

	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}

	if(msg == myDFS_MSG_FAILURE){
		printf("Request to change dir is failed.\n");
		x = read(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			printf("reading from socket.");
			ERROR = myDFS_ESREAD;
		}
		close(sockfd);
		return -1;
	}
	create_path(&path_s, path);
	x = write(sockfd, &path_s, sizeof(path_s));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}

	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}

	x = read(sockfd, &ret, sizeof(ret));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}

	close(sockfd);

	current_dir = path_s;
	return ret;
}
