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
dfs_link(const myDFS_char *oldpath, const myDFS_char *newpath){
	
	myDFS_int	sockfd, len;
	myDFS_int	x, ret = 0;
	myDFS_msg	msg;
	myDFS_LINK_ARG	arg;
	struct sockaddr_in address;

	printf("oldpath : %s\n", oldpath);
	create_path(&(arg.old), oldpath);
	printf("newpath : %s\n", newpath);
	create_path(&(arg.new), newpath);

	msg = myDFS_MSG_LINK;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		chk("socket creation");
		return -1;
	}
	
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
		printf("Request to create a link is failed.\n");
		x = read(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			printf("reading from socket.");
			ERROR = myDFS_ESREAD;
		}
		close(sockfd);
		return -1;
	}

	x = read(sockfd, &ret, sizeof(ret));
	printf("fd %d\n", ret);
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}

	close(sockfd);
	return ret;
}
