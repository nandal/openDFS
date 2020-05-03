#include "../include/dfs_server.h"

int lock_inode(myDFS_inode inode){
	int i;
	for(i=0; i < MAX_FILE_OPEN; i++){
		if(inode_global_pa[i] != NULL && inode_global_pa[i]->id == inode){
			while(inode_global_pa[i]->lock == myDFS_LOCKED){
				usleep(1000);
			}
			inode_global_pa[i]->lock = myDFS_LOCKED;
			return 0;
		}
	}
	return -1;
}
