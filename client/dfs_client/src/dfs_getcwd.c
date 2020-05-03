#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../include/dfs_client.h"

myDFS_char *
dfs_getcwd(myDFS_char *buf, myDFS_size size){
	myDFS_int	sockfd;
	myDFS_int	x;
	myDFS_msg	msg;
	myDFS_path_s	path;
	struct sockaddr_in address;


	msg = myDFS_MSG_GETCWD;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		chk("socket creation");
		return NULL;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(server_ip);
	address.sin_port = htons(server_port);

	x = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if(x == -1){
		chk("connect");
		ERROR = myDFS_ESCONNECT;
		close(sockfd);
		return NULL;
	}
	
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		ERROR = myDFS_ESWRITE;
		close(sockfd);
		return NULL;
	}

	x = write(sockfd, &auth, sizeof(auth));
	if(x == -1){
		chk("writing to socket");
		ERROR = myDFS_ESWRITE;
		close(sockfd);
		return NULL;
	}

	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return NULL;
	}

	if(msg == myDFS_MSG_FAILURE){
		printf("Request to write data to file is failed.\n");
		x = read(sockfd, &ERROR, sizeof(ERROR));
		if(x == -1){
			printf("reading from socket.");
			ERROR = myDFS_ESREAD;
		}
		close(sockfd);
		return NULL;
	}

	x = read(sockfd, &path, sizeof(path));
	if(x == -1){
		chk("reading from socket");
		ERROR = myDFS_ESREAD;
		close(sockfd);
		return NULL;
	}

	close(sockfd);
	strncpy(buf, path.path, size);
	printf("path returned is %s\n", path.path);
	current_dir = path;
	return buf;
}
