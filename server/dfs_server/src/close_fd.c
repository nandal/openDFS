#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/dfs_server.h"

myDFS_int 
close_fd(const myDFS_auth_s *auth_ptr, const myDFS_int fd){
	myDFS_int 	i, ftid, x;
	myDFS_inode	inode;

	printf("TOTAL FDs %d-%d(%d)\n", comp_meta.total_fd, comp_meta.free_fd, MAX_FD);
	printf("TOTAL FT %d-%d(%d)\n", comp_meta.total_ft, comp_meta.free_ft, MAX_FD);
	i = logged_pa[auth_ptr->uid]->fd_index[fd];
	logged_pa[auth_ptr->uid]->total_fd--;
	logged_pa[auth_ptr->uid]->fd_index[fd] = -1;
	ftid = fd_global_pa[i]->ftid;
	free(fd_global_pa[i]);
	fd_global_pa[i] = NULL;
	comp_meta.total_fd--;
	comp_meta.free_fd++;
	printf("Index of fd table %d\n", i);

	inode = ft_global_pa[ftid]->inode;
	free(ft_global_pa[ftid]);
	ft_global_pa[ftid] = NULL;
	comp_meta.total_ft--;
	comp_meta.free_ft++;
	printf("Index of ft table %d\n", ftid);

	x = update_inode_entry(inode_global_pa[inode]);
	if(x != 0){
		chk("updation of inode entry");
	}
	free(inode_global_pa[inode]);
	inode_global_pa[inode] = NULL;
	comp_meta.total_inode--;
	comp_meta.free_inode++;
	printf("%u inode entry is nullified.\n", inode);
	printf("TOTAL FDs %d-%d(%d)\n", comp_meta.total_fd, comp_meta.free_fd, MAX_FD);
	printf("TOTAL FT %d-%d(%d)\n", comp_meta.total_ft, comp_meta.free_ft, MAX_FD);
	return 0;
}
