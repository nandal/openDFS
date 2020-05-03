#include "include/client.h"
#include <string.h>
#include <stdio.h>

int process(const myDFS_char *choice){
	int i;
	if(strncmp(choice, "lsr", 3)==0){
		puts("Here is a list of remote files.");
		list_file(choice);
	}else if(strncmp(choice, "logout", 6)==0){
		puts("Request to logout is sent.");
		dfs_logout();
	}else if(strncmp(choice, "status", 6)==0){
		puts("Request to show status is sent.");
		dfs_status();
	}else if(strncmp(choice, "ls", 2)==0){
		puts("Here is a list of local files.");
		system(choice);
	}else if(strncmp(choice, "cat ", 4)==0){
		puts("The file is being download.");
		read_file(choice);
	}else if(strncmp(choice, "put ", 4)==0){
		puts("The file is being upload.");
		write_file(choice);
	}else if(strncmp(choice, "rm ", 3)==0){
		puts("The local file has been deleted.");
		//delete_file(choice);
	}else if(strncmp(choice, "rmr ", 4)==0){
		puts("The remote file has been deleted.");
		delete_file(choice);
	}else if(strncmp(choice, "mkdir ", 6)==0){
		puts("local dir is created.");
		system(choice);
	}else if(strncmp(choice, "mkdirr ", 7)==0){
		puts("The remote dir is created.");
		make_dir(choice);
	}else if(strncmp(choice, "rmdirr ", 7)==0){
		puts("The remote dir request is sent.");
		remove_dir(choice);
	}else if(strncmp(choice, "stop", 4)==0){
		puts("The server is asked to stop.");
		ask_server_stop();
	}else if(strncmp(choice, "mv ", 3)==0){
		puts("The file is moved.");
		//move_file(choice);
	}else if(strncmp(choice, "lnr ", 4)==0){
		puts("New link is being created.");
		link_file(choice);
	}else if(strncmp(choice, "chmodr ", 7)==0){
		puts("Mode of file is beign chnged");
		chmod_file(choice);
	}else if(strncmp(choice, "chownr ", 7)==0){
		puts("Ownership of file is being changed.");
		chown_file(choice);
	}else if(strncmp(choice, "pwdr", 4)==0){
		getcwd_dir();
		printf("%s\n", current_dir.path);
	}else if(strncmp(choice, "pwd", 3)==0){
		puts("Local Directory's full path.");
		system(choice);
	}else if(strncmp(choice, "cdr", 3)==0){
		puts("Remote Directory is changed.");
		change_dir(choice);
	}else if(strncmp(choice, "cd", 2)==0){
		puts("Local Directory is changed.");
		system(choice);
	}else if(strncmp(choice, "help", 4)==0){
		help();
	}else if(strncmp(choice, "clear", 5)==0){
		system("clear");
	}else{
		puts("Command not recognized.");
		puts("Enter help for options.");
	}
	return 0;
}
