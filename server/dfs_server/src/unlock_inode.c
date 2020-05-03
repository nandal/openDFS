#include "../include/dfs_server.h"

int unlock_inode(myDFS_inode inode){
	int i;
	for(i=0; i < MAX_FILE_OPEN; i++){
		if(inode_global_pa[i] != NULL && inode_global_pa[i]->id == inode){
			inode_global_pa[i]->lock = myDFS_UNLOCKED;
			return 0;
		}
	}
	return -1;
}
