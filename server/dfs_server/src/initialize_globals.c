#include "../include/dfs_server.h"
#include <stdio.h>
#include <stdlib.h>

myDFS_node_s 		slaves[TOTAL_SLAVES]={0};
myDFS_ERROR		ERROR = 0;
/* session related globals */
myDFS_usnode_s		*logged_pa[MAX_SESSIONS]={0};
myDFS_server_status_s	comp_meta;
myDFS_fdnode_s		*fd_global_pa[MAX_FD]={0};
myDFS_ftnode_s		*ft_global_pa[MAX_FD]={0};
myDFS_inode_s 		*inode_global_pa[MAX_NO_FILES]={0};

int initialize_globals(){
	myDFS_int x;
	myDFS_inode_s *inode_p;

	comp_meta.total_logged  = 0;
	comp_meta.free_logged	= MAX_SESSIONS;
	comp_meta.total_fd 	= 0;
	comp_meta.free_fd	= MAX_FD;
	comp_meta.total_ft	= 0;
	comp_meta.free_ft	= MAX_FD;
	comp_meta.total_inode	= 0;
	comp_meta.free_inode	= MAX_NO_FILES;
	inode_p = (myDFS_inode_s *)calloc(1, sizeof(myDFS_inode_s));
	if(inode_p == NULL){
		chk("not enough memory");
		return -1;
	}	
	
	x = read_inode_entry(0, inode_p);
	inode_global_pa[0] = inode_p;

	printf("size of inode table %d\n", sizeof(inode_global_pa));
	printf("size of ft table. %d\n", sizeof(ft_global_pa));
	printf("size of fd table. %d\n", sizeof(fd_global_pa));
	printf("size of session table. %d\n", sizeof(logged_pa));
	puts("All global Variables are initialized.\n");
	printf("total slaves : %d\n", sizeof(slaves) / sizeof(myDFS_node_s));
	return 0;
}
