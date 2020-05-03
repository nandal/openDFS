#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../include/dfs_client.h"


myDFS_ssize
dfs_read(const myDFS_int fd, myDFS_char *buf, const myDFS_ssize len){
	myDFS_int	sockfd;
	myDFS_int	x, i;
	myDFS_ssize	size = 0, left, extra, cycles, ret;
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_msg	msg;
	myDFS_RW_ARG	arg;
	struct sockaddr_in address;

	arg.fd = fd;
	arg.len = len;
	left = len;

	msg = myDFS_MSG_READ;
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

	if(msg == myDFS_MSG_FAILURE){
		printf("Request to open file is failed.\n");
		x = read(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			printf("reading from socket.");
			ERROR = myDFS_ESREAD;
		}
		close(sockfd);
		return -1;
	}
	cycles = arg.len / myDFS_BUFSIZ;
	extra = arg.len % myDFS_BUFSIZ;
	for(i = 0; i < cycles; i++){
		x = read(sockfd, data, myDFS_BUFSIZ);
		strncpy(buf, data, x);
		buf += x;
		size += x;
	}
	x = read(sockfd, data, extra);
	strncpy(buf, data, x);
	size += x; // total bytes read

	close(sockfd);

	return size;
}
