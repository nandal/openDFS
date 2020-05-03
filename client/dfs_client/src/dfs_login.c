#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../include/dfs_client.h"


myDFS_int dfs_login(const myDFS_login_s *login_ptr){
	myDFS_int	sockfd, len;
	myDFS_int	x;
	myDFS_msg	msg;
	struct sockaddr_in address;

	msg = myDFS_MSG_LOGIN;
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
		close(sockfd);
		return -1;
	}
	
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		close(sockfd);
		return -1;
	}

	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		close(sockfd);
		return -1;
	}

	if(msg == myDFS_MSG_FAILURE){
		printf("Request to login is failed.\n");
		close(sockfd);
		return -1;
	}

	x = write(sockfd, login_ptr, sizeof(myDFS_login_s));
	if(x == -1){
		chk("writing to socket");
		close(sockfd);
		return -1;
	}

	x = read(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("reading from socket");
		close(sockfd);
		return -1;
	}
	if(msg == myDFS_MSG_FAILURE){
		printf("\nLogin failed\n");
		close(sockfd);
		return -1;
	}else if(msg == myDFS_MSG_SUCCESS){
		x = read(sockfd, &auth, sizeof(auth));
		if(x == -1){
			chk("reading from socket");
			close(sockfd);
			return -1;
		}
	close(sockfd);
	printf("\nUID : %i\nKEY : %i", auth.uid, auth.key);
	}
	return 0;
}
