#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

myDFS_int 
get_fd(const myDFS_auth_s *auth_ptr, const myDFS_inode inode, myDFS_int flags){
	myDFS_int	retfd, i, x;
	myDFS_inode_s	*inode_p;
	myDFS_ftnode_s	*ftnode_p;
	myDFS_fdnode_s	*fdnode_p;

	inode_p = (myDFS_inode_s *)calloc(1, sizeof(myDFS_inode_s));
	x = read_inode_entry(inode, inode_p);
	printf("Inode entry is read\n");

	free(inode_global_pa[inode]);
	inode_global_pa[inode] = inode_p;
	comp_meta.total_inode++;
	comp_meta.free_inode--;

	ftnode_p = (myDFS_ftnode_s *)calloc(1, sizeof(myDFS_ftnode_s));
	ftnode_p->flags = flags;
	ftnode_p->count = 1;
	ftnode_p->inode = inode;
	for(i = 0; i < MAX_FD; i++){
		puts("loop ftnode");
		if(ft_global_pa[i] == NULL){
			printf("%i index is assigned of ftnode\n", i);
			ft_global_pa[i] = ftnode_p;
			comp_meta.total_ft++;
			comp_meta.free_ft--;
			break;
		}
	}
	if(i == MAX_FD){
		puts("System cant open more files.");
		free(inode_p);
		free(ftnode_p);
		return -1;
	}

	fdnode_p = (myDFS_fdnode_s *)calloc(1, sizeof(myDFS_fdnode_s));
	fdnode_p->ftid = i;
	fdnode_p->offset = 0;
	fdnode_p->flags = flags;

	for(i = 0; i < MAX_FD; i++){
		puts("loop fdnode");
		if(fd_global_pa[i] == NULL){
			printf("%i index is assigned of fdnode\n", i);
			fd_global_pa[i] = fdnode_p;
			comp_meta.total_fd++;
			comp_meta.free_fd--;
			break;
		}
	}
	if(i == MAX_FD){
		puts("System cant create more fd\n");
		free(inode_p);
		free(ft_global_pa[fdnode_p->ftid]);
		free(fdnode_p);
		return -1;
	}

	retfd = logged_pa[auth_ptr->uid]->total_fd++;
	logged_pa[auth_ptr->uid]->fd_index[retfd] = i;
	printf("%d fd is assigned\n", retfd);
	printf("TOTAL FDs %d-%d(%d)\n", comp_meta.total_fd, comp_meta.free_fd, MAX_FD);
	printf("TOTAL FT %d-%d(%d)\n", comp_meta.total_ft, comp_meta.free_ft, MAX_FD);

	return retfd;
}

