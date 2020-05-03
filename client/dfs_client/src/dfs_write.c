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
dfs_write(const myDFS_int fd, const myDFS_char *buf, const myDFS_ssize len){
	myDFS_int	sockfd;
	myDFS_int	x, i, cycles;
	myDFS_ssize	size = 0, left = 0, ssize; // left means the total data has to be recieved
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_msg	msg;
	myDFS_RW_ARG	arg;
	struct sockaddr_in address;

	arg.fd = fd;
	arg.len = len;
	left = len;

	msg = myDFS_MSG_WRITE;
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
		printf("Request to write data to file is failed.\n");
		x = read(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			printf("reading from socket.");
			ERROR = myDFS_ESREAD;
		}
		close(sockfd);
		return -1;
	}
	cycles = len / myDFS_BUFSIZ;
	left = len % myDFS_BUFSIZ;
	for(i = 0; i < cycles; i++){
		x = write(sockfd, buf, myDFS_BUFSIZ);
//		write(1, buf, myDFS_BUFSIZ);
		buf += x;
		size += x;
	}
	x = write(sockfd, buf, left);
//	x = write(1, buf, left);
	size += x;
	
	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return -1;
	}

	if(msg == myDFS_MSG_FAILURE){
		printf("write data to file is failed.\n");
		x = read(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			printf("reading from socket.");
			ERROR = myDFS_ESREAD;
		}
		close(sockfd);
		return -1;
	}
	x = read(sockfd, &ssize, sizeof(ssize));
	if(size ==  ssize)puts("all data is written cheked.");
	close(sockfd);
	return size;
}
