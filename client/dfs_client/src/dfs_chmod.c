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
dfs_chmod(const myDFS_char *path, myDFS_mode mode){

	myDFS_int	sockfd, len;
	myDFS_int	x, ret = 0;
	myDFS_msg	msg;
	myDFS_CREAT_ARG	arg;
	struct sockaddr_in address;

	printf("path : %s\n", path);
	create_path(&(arg.path), path);
	arg.mode = mode;

	msg = myDFS_MSG_CHMOD;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		chk("socket creation");
		return -1;
	}
	printf("Path : %s\n", arg.path.path);
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(server_ip);
	address.sin_port = htons(server_port);
	len = sizeof(address);

	x = connect(sockfd, (struct sockaddr *)&address, len);
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

	x = write(sockfd, &arg, sizeof(arg));
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
	puts("Request is successful");

	if(msg == myDFS_MSG_FAILURE){
		printf("Request to chmod is failed.\n");
		x = read(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			printf("reading from socket.");
			ERROR = myDFS_ESREAD;
		}
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
	return ret;
}
