#include "../include/dfs_server.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

int login_file(myDFS_int sockfd, const myDFS_node_s *node_ptr){
	myDFS_int	fd, x, j;
	myDFS_msg	msg;
	myDFS_uid	i;
	myDFS_time	t;
	myDFS_login_s	loginDetail;
	myDFS_session_s	session;
	myDFS_uhead_s	uhead;
	myDFS_unode_s	unode;
	myDFS_auth_s	auth;
	myDFS_usnode_s	*usnode_p;
	myDFS_char	file[MAX_FILE_NAME];

	usnode_p = (myDFS_usnode_s *)calloc(1, sizeof(myDFS_usnode_s));
	if(usnode_p == NULL){
		chk("calloc usnode_p");
		return -1;
	}
	for(i = 0; i < MAX_FD_PER_USER; i++){
		usnode_p->fd_index[i] = -1;
	}


	msg = myDFS_MSG_SUCCESS;
	x = write(sockfd, &msg, sizeof(msg));

	x = read(sockfd, &loginDetail, sizeof(loginDetail));
	if(x == -1){
		chk("reading fhead from socket");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
		}
		return -1;
	}
	sprintf(file, "%s/user.tbl", SERVER_DATALOG);
	fd = open(file, O_RDONLY);
	if(fd == -1){
		chk("opening user table");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
		}
		return -1;
	}
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek the user table head");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
		}
		close(fd);
		return -1;
	}
	x = read(fd, &uhead, sizeof(uhead));
	if(x == -1){
		chk("reading uhead from user table");
		msg = myDFS_MSG_FAILURE;
		x = write(sockfd, &msg, sizeof(msg));
		if(x == -1){
			chk("writing to socket");
		}
		close(fd);
		return -1;
	}
	for(i = 0; i < MAX_USERS-uhead.free;){
		x = read(fd, &unode, sizeof(unode));
		if(x == -1){
			chk("reading unode from user table");
			msg = myDFS_MSG_FAILURE;
			x = write(sockfd, &msg, sizeof(msg));
			if(x == -1){
				chk("writing to socket");
			}
			close(fd);
			return -1;
		}
		if(strlen(unode.name) != 0)i++;
		if(strcmp(unode.name, loginDetail.name) == 0){
			if(strcmp(unode.pass, loginDetail.pass) == 0){
				close(fd);
				usnode_p->key = rand(); /* assigning a key to the user */
				sprintf(usnode_p->current.path, "/home/%s/", unode.name);
				create_path(&usnode_p->current, usnode_p->current.path);
				strcpy(usnode_p->ip, node_ptr->ip);
				/*creating session entry in global space */
				free(logged_pa[unode.id]);
				logged_pa[unode.id] = usnode_p;
				comp_meta.total_logged++;
				comp_meta.free_logged--;
				printf("Key is assigned n login successful\n");
				msg = myDFS_MSG_SUCCESS;
				x = write(sockfd, &msg, sizeof(msg));
				if(x == -1){
					chk("writing to socket");
					return -1;
				}
				auth.key = usnode_p->key;
				auth.uid = unode.id;
				x = write(sockfd, &auth, sizeof(auth));
				if(x == -1){
					chk("writing to socket");
					return -1;
				}
	printf("Total logged in are %u\n", comp_meta.total_logged);

				/* making entry login tables for this session */
				sprintf(file, "%s/session/login.tbl", SERVER_DATALOG);
				fd = open(file, O_WRONLY|O_CREAT, 0666);
				if(fd == -1){
					chk("opening login table");
					close(fd);
					return -1;
				}
				x = lseek(fd, 0, SEEK_END);
				if(x == -1){
					chk("lseek the end of login detail table");
					close(fd);
					return -1;
				}
				session.uid = unode.id;
				time(&session.time);
				session.port = node_ptr->port;
				strcpy(session.ip, node_ptr->ip);
				x = write(fd, &session, sizeof(session));
				if(x == -1){
					chk("entering login info in login table");
					close(fd);
					return -1;
				}
				close(fd);
				return 0;			
			}else{
				printf("login failed.\n");
				msg = myDFS_MSG_FAILURE;
				x = write(sockfd, &msg, sizeof(msg));
				if(x == -1){
					chk("writing to socket");
					return -1;
				}
				close(fd);
				return 0;
			}
		}
	}
	close(fd);
	msg = myDFS_MSG_FAILURE;
	x = write(sockfd, &msg, sizeof(msg));
	if(x == -1){
		chk("writing to socket");
		return -1;
	}
	printf("login failed.\n");
	return 0;
}
