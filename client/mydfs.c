#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/client.h"


myDFS_char 	prompt[MAX_PATH_SIZE];

int main(int argc, char *argv[]){
	myDFS_int 	i, x, fd;
	myDFS_char	pass[50];
	myDFS_char 	choice[50], data[myDFS_BUFSIZ]="";
	myDFS_login_s	loginDetail;

	dfs_initialize();

	create_path(&current_dir, "/");

	for(i=1; i<argc; i++){
		if(strcmp(argv[i], "-ip") == 0){
			strcpy(server_ip, argv[i+1]);
		}else if(strcmp(argv[i], "-p") == 0){
			server_port = atoi(argv[i+1]);
		}
	}
	system("clear");
	printf("Welcome to myDFS [ip:%s]:[port:%i]\n", server_ip, server_port);
go:
	printf("Enter Username:");
	fgets(user, MAX_USERNAME, stdin);
	user[strlen(user)-1] = '\0';
	strcpy(loginDetail.name, user);

	printf("Enter Password:");
	system("stty -echo");
	fgets(loginDetail.pass, MAX_PASSWORD, stdin);
	loginDetail.pass[strlen(loginDetail.pass)-1] = '\0';
	system("stty echo");

	if(dfs_login(&loginDetail)!=0){
		printf("Login Attempt Failed\n");
		goto go;
	}
	bzero(&loginDetail,sizeof(loginDetail));

	if(strcmp(user, "root") == 0 && auth.uid == 0){
		strcpy(current_dir.path, "/");
		sprintf(prompt, "%s@myDFS />#", user);
	}else{
		sprintf(current_dir.path, "/home/%s/", user);
		sprintf(prompt, "%s@myDFS ~/>$", user);
	}
	printf("\n");
	while(1){
		printf("%s", prompt);
		fgets(choice, 50, stdin);
		choice[strlen(choice)-1] = '\0';
		if(strncmp(choice, "exit", 4) == 0)break;
		x = process(choice);
//		printf("Enter data : \n");
//		fgets(data, myDFS_BUFSIZ, stdin);
//		x = dfs_write(fd, data, strlen(data));
//		printf("%d bytes are writen to server.\n", x);
	}

	return 0;
	
}
