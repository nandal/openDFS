#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "include/client.h"

int ask_server_stop(){
	myDFS_int	sockfd, x, len;
	myDFS_msg	msg;
	struct sockaddr_in address;

	msg = myDFS_MSG_STOP;

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
		chk("writing of stop request on socket failed");
		close(sockfd);
		return -1;
	}
	close(sockfd);
	printf("Request to stop the server is sent.\n");
	return 0;
}
