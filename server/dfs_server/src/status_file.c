#include "../include/dfs_server.h"
#include <stdio.h>

myDFS_int 
status_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr){
	myDFS_int i,j;

	printf("USERS LOGGED IN %d-%d(%d)\n", comp_meta.total_logged, comp_meta.free_logged, MAX_SESSIONS);
	printf("UID\tKEY\t\tCURRENT\t\tIP\t\tTOTAL FD\tFD IDs\n");
	for(i = 0; i < MAX_SESSIONS; i++){
		if(logged_pa[i] != NULL){
			printf("%u\t%i\t%s\t%s\t%i\t\t:", i, logged_pa[i]->key, logged_pa[i]->current.path, logged_pa[i]->ip, logged_pa[i]->total_fd);
			for(j = 0; j < logged_pa[i]->total_fd;){
				if(logged_pa[i]->fd_index[j] != -1){
					printf("%u, ", logged_pa[i]->fd_index[j]);	
					j++;
				}
			}
			printf("\n");
		}
	}	

	printf("TOTAL FDs %d-%d(%d)\n", comp_meta.total_fd, comp_meta.free_fd, MAX_FD);
	printf("FD INDEX\tOFFSET\tFLAGS\tFTID\n");
	for(i = 0; i < MAX_FD; i++){
		if(fd_global_pa[i] != NULL){
			printf("%i\t\t%u\t%i\t%i\n", i, fd_global_pa[i]->offset, fd_global_pa[i]->flags, fd_global_pa[i]->ftid);
		}	
	}

	printf("TOTAL FT %d-%d(%d)\n", comp_meta.total_ft, comp_meta.free_ft, MAX_FD);
	printf("FTID\tLOCK\tFLAGS\tINODE\tCOUNT\n");
	for(i = 0; i < MAX_FD; i++){
		if(ft_global_pa[i] != NULL)
		printf("%i\t%i\t%i\t%u\t%i\n", i, ft_global_pa[i]->lock, ft_global_pa[i]->flags, ft_global_pa[i]->inode, ft_global_pa[i]->count);
	}	

	printf("TOTAL SLAVES %d\n", TOTAL_SLAVES);
	printf("IP:PORT");
	for(i = 0; i < TOTAL_SLAVES; i++){
		printf("%s:%i\n", slaves[i].ip, slaves[i].port);
	}
	return 0;
}
