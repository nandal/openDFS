#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "header/slave.h"

int slave_make_entry_slave_list(const myDFS_node_s *slave_node_ptr){
	myDFS_int	server_sockfd;
	myDFS_int	len;
	myDFS_int	result;
	myDFS_msg_s	msg;
	struct sockaddr_in server_address;

	msg.type = myDFS_MSG_HI_SLAVE;
	printf("SLAVE say hi to IP : %s\n", SERVER_IP);

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sockfd == -1){
		printf("socket creation is failed to talk with server.\n");
		exit(-1);
	}
	
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_address.sin_port = htons(SERVER_PORT);
	len = sizeof(server_address);

	result = connect(server_sockfd, (struct sockaddr *)&server_address, len);
	perror("connected to server to say hi");
	write(server_sockfd, &msg, sizeof(myDFS_msg_s));
	perror("hi is writen to socket");
	write(server_sockfd, slave_node_ptr, sizeof(myDFS_node_s));
	perror("node info is written to socket.");
	read(server_sockfd, &msg, sizeof(myDFS_msg_s));
	perror("hello is read from socket.");
	if(msg.type == myDFS_MSG_HELLO_SERVER){
		puts("Handshaked with server.");
	}
	close(server_sockfd);
	return 0;
}
