#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "include/slave.h"

int main(int argc, char *argv[]){
	myDFS_int 	server_sockfd, client_sockfd;
	myDFS_int	server_len, client_len;
	myDFS_node_s	slave_node;
	myDFS_int x;

	struct sockaddr_in client_address;
	struct sockaddr_in server_address;
	


	perror("slave.c:23:");
	slave_read_config(&slave_node);
	perror("slave.c:25");
	//slave_make_entry_slave_list(&slave_node);

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	perror("server_socket");

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(slave_node.ip);
	server_address.sin_port = htons(slave_node.port);

	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	listen(server_sockfd, 5);
	while(1){
		myDFS_msg_s msg;
        	printf("slave is listening to IP : %s\n", slave_node.ip);
	        printf("Port of slave is : %i\n", slave_node.port);

		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		printf("Client IP is : %s\n", inet_ntoa(client_address.sin_addr));	
		printf("Client port is : %i\n", ntohs(client_address.sin_port));
		read(client_sockfd, &msg, sizeof(myDFS_msg_s));
		perror("reading message req");
		slave_process_request(client_sockfd, &msg);
		close(client_sockfd);
	}	
	return 0;
}

