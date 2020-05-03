#include <stdio.h>
#include <string.h>
#include "include/client.h"


myDFS_char * get_filename(const myDFS_char *path){
	myDFS_int	index=0;
	index = strlen(path);
	while(index-- > 0){
		if(*(path+index) == '/')break;
	}
	return (char *)(path+index+1);
}

/*
int main(int argc, char *argv[]){
	myDFS_path_s 	cur;
	myDFS_char	filename[MAX_FILE_NAME];

	strcpy(cur.path, argv[1]);
	strcpy(filename, get_filename(cur.path));
	if(strlen(filename)== 0)puts("DIR");
	printf("File : %s\n", filename);
	return 0;
}
*/
