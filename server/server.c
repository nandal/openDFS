#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "include/server.h"

int main(int argc, char *argv[]){
	myDFS_int 	server_sockfd, client_sockfd;
	myDFS_int 	server_len, client_len;
	myDFS_int 	i=0, fd, x;
	myDFS_msg 	msg;
	myDFS_node_s	node;

	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	initialize_globals();

	printf("Server IP : %s\n", SERVER_IP);
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sockfd == -1){
		chk("socket creation");
		return -1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_address.sin_port = htons(SERVER_PORT);

	server_len = sizeof(server_address);
	x = bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	if(x == -1){
		chk("binding of socket");
		return -1;
	}

	x = listen(server_sockfd, 5);
	if(x == -1){
		chk("listening by socket");
		return -1;
	}

	fd = open("datalogs/slave.tbl", O_RDONLY);
	if(fd == -1){
		chk("opening slave.tbl");
		return -1;
	}
	for(i=0 ; i<TOTAL_SLAVES; i++){
		printf("Server waiting for slave[%d].\n", i);
		x = read(fd, &slaves[i], sizeof(myDFS_node_s));
		if(x == -1){
			chk("reading slave info from slave.tbl");
		}
		printf("Slave IP is : %s\n", slaves[i].ip);	
		printf("Slave port is : %i\n", ntohs(slaves[i].port));
	}
		close(fd);

	/* Now server is ready with its slaves waiting for clients */
	while(1){
		printf("Server waiting.\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		if(client_sockfd == -1){
			chk("accepting of client socket");
			return -1;
		}
		printf("Client IP is : %s\n", inet_ntoa(client_address.sin_addr));	
		printf("Client port is : %i\n", ntohs(client_address.sin_port));
		strcpy(node.ip, inet_ntoa(client_address.sin_addr));
		node.port = ntohs(client_address.sin_port);
		x = read(client_sockfd, &msg, sizeof(myDFS_msg));
		if(x == -1){
			chk("reading from client socket");
			return -1;
		}
		if(msg == myDFS_MSG_LOGIN){
			printf("login request\n");
			login_file(client_sockfd, &node);
		}else{
			process_request(client_sockfd, msg);
		}
		close(client_sockfd);
	}
	return 0;
}
