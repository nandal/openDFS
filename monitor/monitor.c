#include "../server/include/server.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int show_inode_table();
int show_name_table(myDFS_inode dir_id);
int show_data_table();
int show_slave_table();
int read_file_warehouse(char *);
int first_time_run_only();
int show_session(myDFS_uid uid);
int main(){
	int choice = -1;
	myDFS_uid uid;
	myDFS_inode inode;
	char filename[50];
	while(choice != 0){
		printf("1) INODE TBL ");
		printf("2) NAME TBL ");
		printf("3) DATA BLK TBL ");
		printf("4) SLAVE TBL ");
		printf("5) FORMAT\n");
		printf("6) LOGIN LOG ");
		printf("7) FILE TBL ");
		printf("8) USER TBL ");
		printf("9) GROUP TBL ");
		printf("10) SESSION\n");
		scanf("%d", &choice);
		system("clear");
		switch (choice){
			case 0:
				exit(0);
			case 1:
				show_inode_table();
				break;
			case 2:
				printf("Enter the dir inode:");
				scanf("%u", &inode);
				show_name_table(inode);
				break;
			case 3:
				show_data_table();
				break;
			case 4:
				show_slave_table();
				break;
			case 5: 
				first_time_run_only();
				break;
			case 6: 
				show_login_log();
				break;
			case 7: 
				//show_file_table();
				break;
			case 8: 
				show_user_table();
				break;
			case 9: 
				show_group_table();
				break;
			case 10:
				printf("Enter the user id:");
				scanf("%u", &uid);
				//show_session(uid);
				break;
			default:
				printf("Not valid input\n");
		}
	}
	return 0;
}
int show_inode_table(){
	int fd, x;
	myDFS_blk i,j;myDFS_char ch;
	myDFS_inode_s 	inode;
	myDFS_ihead_s	ihead;
	fd = open("../server/datalogs/inode.tbl", O_RDONLY);
	x = read(fd, &ihead, sizeof(myDFS_ihead_s));
	printf("\n**************INODE TABLE [%u]*******************\n", ihead.free);
	printf("id\t:size\t:nlink\t:mode\t:uid\t:gid\t:blocks\t:data\n");
	for(j = 0; j < MAX_NO_FILES - ihead.free;){
		x = read(fd, &inode, sizeof(inode));
		if(inode.nlink > 0)j++;
		ch = myDFS_IS_DIR(inode.mode)?('*'):(' ');
		printf("%u%c\t:%u\t:%i\t:%o\t:%u\t:%u\t:%i\t:", inode.id, ch, inode.size, inode.nlink, inode.mode, inode.uid, inode.gid, inode.blocks);
		for(i = 0; i < inode.blocks; i++)printf("%u, ",inode.data[i]);
		printf("\t:atime:%u\t mtime:%u\t ctime:%s ", inode.atime, inode.mtime, ctime(&inode.ctime));
	}
	printf("\n**************INODE TABLE [%u]*******************\n", ihead.free);
	close(fd);
	return 0;
}
int show_name_table(myDFS_inode dir_id){
	int fd, x;
	myDFS_blk i,j;
	myDFS_nnode_s 	nnode;
	myDFS_nhead_s	nhead;
	myDFS_char 	dir[50];

	sprintf(dir, "../server/datalogs/dir/%u.tbl", dir_id);
	fd = open(dir, O_RDONLY);
	x = read(fd, &nhead, sizeof(nhead));
	printf("\n**************DIR TABLE [%u]*******************\n", nhead.free);
	printf("\n*****PARENT ID [%u] OWN ID [%u]****************\n", nhead.parent, nhead.self);
	printf("inode : name\n");
	for(j = 0; j < MAX_DIR_ENTRY; j++){
		x = read(fd, &nnode, sizeof(nnode));
		if(strlen(nnode.name) != 0)
			printf("%u : %s\n", nnode.inode, nnode.name);
	}
	printf("\n**************DIR TABLE [%u]*******************\n", nhead.free);
	close(fd);
	return 0;
}
int show_data_table(){
	int fd, x;
	myDFS_blk i,j;
	myDFS_dnode_s 	dnode;
	myDFS_dhead_s	dhead;
	fd = open("../server/datalogs/data_block.tbl", O_RDONLY);
	x = read(fd, &dhead, sizeof(myDFS_dhead_s));
	printf("\n**************DATA BLOCK TABLE [%u]*******************\n", dhead.free);
	printf("id\tslave_id\tsub_id\tstatus\n");
	for(j = TOTAL_SLAVES * SLAVE_CAPACITY; j > dhead.free; j--){
		x = read(fd, &dnode, sizeof(myDFS_dnode_s));
		printf("%u\t%u\t\t%u\t%u\n", dnode.id, dnode.slave_id, dnode.sub_id, dnode.status);
	}
	printf("\n**************DATA BLOCK TABLE [%u]*******************\n", dhead.free);
	close(fd);
	return 0;
}
int show_slave_table(){
	int fd, x;
	myDFS_node_s 	node;
	fd = open("../server/datalogs/slave.tbl", O_RDONLY);
	printf("\n************** SLAVE TABLE *******************\n");
	printf("ip\t:\tport\n");
	while(x = read(fd, &node, sizeof(myDFS_node_s))){
		printf("%s\t: %i\n", node.ip, node.port);
	}
	printf("\n************** SLAVE TABLE *******************\n");
	close(fd);
	return 0;
}
int read_file_warehouse(char *filename){
	int 		fd, x;
	myDFS_char 	data[myDFS_BUFSIZ];
	fd = open(filename, O_RDONLY);
	if(fd == -1)perror("opening file");
	printf("\n************** File Data *******************\n");
	while(x = read(fd, data, myDFS_BUFSIZ)){
		write(1, data, x);
	}
	printf("\n************** File End *******************\n");
	close(fd);
	return 0;
}
int show_login_log(){
	int fd, x;
	myDFS_session_s ses;
	fd = open("../server/datalogs/session/login.tbl", O_RDONLY);
	printf("\n**************LOGIN SESSIONS TABLE *******************\n");
	printf("uid\tmachine(ip:port)\ttime\n");
	while(x = read(fd, &ses, sizeof(ses))){
		printf("%u\t[%s:%i]\t%s", ses.uid, ses.ip, ses.port, ctime(&ses.time));
	}
	printf("\n**************LOGIN SESSIONS TABLE*******************\n");
	close(fd);
	return 0;
}/*
int show_file_table(){
	int x, j;
	myDFS_ftnode_s *ftnode_p;
	printf("\n**************CURRENTLY OPENED FILE [%u]*******************\n", comp_meta.free_ft);
	printf("inode\tflags\tcount\n");
	for(j = MAX_FILE_OPEN; j > comp_meta.free_ft; j--){
		ftnode_p = ft_global_pa[j];
		if(ftnode_p != NULL)
		printf("%u\t%i\t%i\n", ftnode_p->inode, ftnode_p->flags, ftnode_p->count);
	}
	printf("\n**************CURRENTLY OPENED FILE [%u]*******************\n", comp_meta.free_ft);
	return 0;
	
}*/

int show_user_table(){
	int fd, x;
	myDFS_unode_s 	unode;
	myDFS_uhead_s	uhead;
	fd = open("../server/datalogs/user.tbl", O_RDONLY);
	x = read(fd, &uhead, sizeof(uhead));
	printf("\n************** USER TABLE [%u]*******************\n", uhead.free);
	printf("uid\t:username\t:password\t\n");
	while(x = read(fd, &unode, sizeof(unode))){
		printf("%u\t:%s\t:%s\n", unode.id, unode.name, unode.pass);
	}
	printf("\n************** USER TABLE [%u] *******************\n", uhead.free);
	close(fd);
	return 0;
}
int show_group_table(){
	int fd, x;
	myDFS_gnode_s 	gnode;
	myDFS_ghead_s	ghead;
	fd = open("../server/datalogs/group.tbl", O_RDONLY);
	x = read(fd, &ghead, sizeof(ghead));
	printf("\n************** GROUP TABLE [%u]*******************\n", ghead.free);
	printf("gid\t:group name\n");
	while(x = read(fd, &gnode, sizeof(gnode))){
		printf("%u\t:%s\n", gnode.id, gnode.name);
	}
	printf("\n************** GROUP TABLE [%u] *******************\n", ghead.free);
	close(fd);
	return 0;
}
/*
int show_session(myDFS_uid uid){
	int i;
	myDFS_usnode_s  *usnode_p;
	myDFS_fdnode_s	*fdnode_p;

	usnode_p = logged_pa[uid];

	printf("\n************** USER SESSION TABLE [%u] *******************\n", uid);
	printf("fd\t:offset\t:flags\t:ftid\n");
	for(i = 0; i < usnode_p->total_fd; i++){
		fdnode_p = fd_global_pa[usnode_p->fd_index[i]];
		printf("%i\t:%u\t:%i\t:%u\n", i, fdnode_p->offset, fdnode_p->flags, fdnode_p->ftid);
	}
	printf("*UID[%u] KEY[%u] LIMIT[%u] IP[%s] CURRENT[%s]*\n", uid, usnode_p->key, usnode_p->total_fd, usnode_p->ip, usnode_p->current.path);
	return 0;
}*/
