#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include "header/slave.h"

int slave_read_config(myDFS_node_s *slave_node_ptr){
	myDFS_int	fd, x, i=0;
	myDFS_char	ch;
	myDFS_char	line[myDFS_BUFSIZ];

	bzero(line, myDFS_BUFSIZ);

	fd = open("conf/slave.confg", O_RDONLY);
	perror("opening:slave_read_config.c:12");
	while(x = read(fd, &ch, 1)){
		if(ch == '='){
			/* READING SLAVE_IP FROM CONFIG */
			if(strcmp(line, "SLAVE_IP")==0){
				printf("%s=", line);
				i = 0; 
				bzero(line, strlen(line));

				while(x = read(fd, &ch, 1) && ch != '\n'){
					line[i++] = ch;
				}
				strcpy(slave_node_ptr->ip, line);
				printf("%s\n", line);
				i = 0; 
				bzero(line, strlen(line));
			}
			/* READING SLAVE_PORT FROM CONFIG */
			else if(strcmp(line, "SLAVE_PORT")==0){
				printf("%s=", line);
				i = 0; 
				bzero(line, strlen(line));

				while(x = read(fd, &ch, 1) && ch != '\n'){
					line[i++] = ch;
				}
				slave_node_ptr->port = (short)atoi(line);
				printf("%s\n", line);
				i = 0; 
				bzero(line, strlen(line));
			}
			continue;
		}else{
			line[i++] = ch;
		}
	}
	close(fd);
	return 0;
}
