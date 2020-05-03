#include "include/server.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


int dispatch_data(myDFS_char *dir, myDFS_dev slave_id);
int write_file_dis(char *filename, myDFS_dev slave_id);

int main(int argc, char *argv[]){
	myDFS_char	dir[50];
	int i;
	
	for(i = 0; i < TOTAL_SLAVES; i++){
		sprintf(dir, "/HD/storage/project/myDFS/server/datawarehouse/%u", i);
		printf("DIR\n\n");
		dispatch_data(dir, i);
	}
	printf("DONE\n");
	
	return 0;
}

int dispatch_data(myDFS_char *dir, myDFS_dev slave_id){

	DIR *dp;
	int fd, x;
	char data[myDFS_BUFSIZ];
	struct dirent *entry;
	struct stat statbuf;
	printf("Slave id : %u\n", slave_id);
	if((dp = opendir(dir)) == NULL) {
		fprintf(stderr,"cannot open directory: %s\n", dir);
		return -1;
	}
	chdir(dir);
	while((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name,&statbuf);
		if(S_ISREG(statbuf.st_mode)) {
		/* READING ONLY REGULAR FILES */
			printf("FILE:%s:DIR:%u\n",entry->d_name, slave_id);
			x = write_file_dis(entry->d_name, slave_id);
			if(x == 0){
				unlink(entry->d_name);
				printf("The file %s is sent to the %i slave storage\n", entry->d_name, slave_id);
			}
		}
	}chdir("../");
	closedir(dp);
	return 0;
	
}


int write_file_dis(char *filename, myDFS_dev slave_id){
	myDFS_int	fd, sockfd, len;
	myDFS_int	x, total=0;
	myDFS_char	data[myDFS_BUFSIZ];
	myDFS_int 	result;
	myDFS_msg	msg;
	myDFS_fhead_s	fhead;
	myDFS_size	size = 0;
	char 		ip[50];
	struct sockaddr_in address;

	if(slave_id == 0){
		strcpy(ip, "192.168.56.1");
	}else{
		strcpy(ip, "192.168.56.101");
	}
	strcpy(fhead.name, filename);

	msg = myDFS_MSG_WRITE;
	fd = open(filename, O_RDONLY);
	perror("opening file");
	fhead.size = lseek(fd, 0, SEEK_END);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(SLAVE_PORT);
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len);
	perror("connect");
	
	/* WRITE REQUEST */
	write(sockfd, &msg, sizeof(myDFS_msg));
	read(sockfd, &msg, sizeof(myDFS_msg));
	if(msg == myDFS_MSG_FAILURE){
		printf("Failure Step 1.\n");
		return -1;
	}

	/* FILE HEADER */
	write(sockfd, &fhead, sizeof(myDFS_fhead_s));
	read(sockfd, &msg, sizeof(myDFS_msg));
	if(msg == myDFS_MSG_FAILURE){
		printf("Failure Step 2.\n");
		return -1;
	}
	/* file info */
	printf("Filename : %s\n", filename);
	printf("File size : %u\n", fhead.size);
	/* FILE DATA */
	lseek(fd, 0, SEEK_SET);
	while(x = read(fd, data, myDFS_BUFSIZ)){
		size += x;
		if(x < myDFS_BUFSIZ){
			myDFS_char padding[myDFS_BUFSIZ];
			bzero(padding, myDFS_BUFSIZ);
			write(sockfd, data, x);
			write(sockfd, padding, myDFS_BUFSIZ - x);
			size += myDFS_BUFSIZ - x;
			break;
		}
		write(sockfd, data, x);
	}
	close(fd);
	printf("data %u of file is written.\n", size);
	printf("Waiting for confirmation %u from SLAVE\n", x);
	read(sockfd, &msg, sizeof(myDFS_msg));
	if(msg == myDFS_MSG_FAILURE){
		printf("Failure Step 3.\n");
		close(sockfd);
		return -1;
	}

	close(sockfd);
	printf("Type of msg returned : %d\n", msg);
	if(msg == myDFS_MSG_SUCCESS)
		return 0;
	else
		return -1;
}

