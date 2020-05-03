#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../server/include/server.h"

int first_time_run_only(){
	myDFS_int 	fd, x;
	myDFS_iid	id, i, j, k;
	myDFS_nhead_s 	nhead;
	myDFS_nnode_s	nnode;

	myDFS_ihead_s 	ihead;
	myDFS_inode_s	inode;

	myDFS_uhead_s	uhead;
	myDFS_unode_s	unode;

	myDFS_ghead_s 	ghead;
	myDFS_gnode_s	gnode;

	myDFS_dhead_s	dhead;
	myDFS_dnode_s	dnode;
	
	myDFS_char 	dirname[50];
	myDFS_node_s	node;

	myDFS_time	t;

	/* reseting inode.tbl */
	fd = open("../server/datalogs/inode.tbl", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("inode.tbl");
	}
	ihead.free = MAX_NO_FILES;
	bzero(inode.data, sizeof(inode.data));
	inode.mode = 0;
	inode.size = 0;
	inode.uid = 0;
	inode.gid = 0;
	inode.nlink = 0;
	inode.blocks = 0;
	inode.atime = 0;
	inode.ctime = 0;
	inode.mtime = 0;
	inode.lock = 0;
	lseek(fd, 0, SEEK_SET);
	x = write(fd, &ihead, sizeof(myDFS_ihead_s));
	if(x == -1){
		chk("writing head to inode.tbl");
	}
	for(i = 0; i < MAX_NO_FILES; i++){
		inode.id = i;
		write(fd, &inode, sizeof(myDFS_inode_s));
	}
	x = close(fd);
	if(x == -1){
		chk("closing inode.tbl");
	}
	puts("INODE TABLE IS RESET");


	/* reseting name table dir.tbl */
	
	system("rm -r ../server/datalogs/dir/*");
	x = rmdir("../server/datalogs/dir");
	if(x == -1){
		chk("unlinking dir dir");
	}
	x = mkdir("../server/datalogs/dir", 0777);
	if(x == -1){
		chk("creating dir dir");
	}
	puts("DIR DIR CREATED");
	fd = open("../server/datalogs/dir/0.tbl", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("creating root 0.tbl");
	}
	nhead.free = MAX_DIR_ENTRY;
	nhead.self = 0;
	nhead.parent = 0;
	bzero(nnode.name, MAX_FILE_NAME);
	nnode.inode = 0;
	lseek(fd, 0, SEEK_SET);
	x = write(fd, &nhead, sizeof(myDFS_nhead_s));
	if(x == -1){
		chk("writing to 0.tbl");
	}
	for(i = 0; i < MAX_DIR_ENTRY; i++){
		write(fd, &nnode, sizeof(myDFS_nnode_s));
	}
	x = close(fd);
	if(x == -1){
		chk("closing 0.tbl");
	}
	puts("ROOT DIR CREATED IN DIR DIR");

	/* creating a root dir entry in inode */
	inode.id = 0;
	inode.mode = myDFS_MODE_DIR | 0777;
	inode.size = 0;
	inode.nlink = 1;
	inode.uid = 0;
	inode.gid = 0;	
	inode.blocks = 0;
	time(&t); /* geting the current time of system */
	inode.ctime = t;
	inode.atime = t;
	inode.mtime = t;
	bzero(inode.data, sizeof(inode.data));
	fd = open("../server/datalogs/inode.tbl", O_RDWR);
	if(fd == -1){
		chk("opening inode.tbl");
	}
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek 0 of inode.tbl");
	}
	x = read(fd, &ihead, sizeof(myDFS_ihead_s));
	if(x == -1){
		chk("reading inode.tbl");
	}
	x = write(fd, &inode, sizeof(myDFS_inode_s));
	if(x == -1){
		chk("writing to inode.tbl");
	}
	ihead.free--;
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek 0 inode.tbl");
	}
	x = write(fd, &ihead, sizeof(myDFS_ihead_s));
	if(x == -1){
		chk("writing to inode.tbl");
	}
	x = close(fd);
	if(x == -1){
		chk("closing inode.tbl");
	}
	puts("ROOT DIR ENTRY IS MADE IN INODE TABLE");
	

	/* reseting slave.tbl */
	fd = open("../server/datalogs/slave.tbl", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("opening slave.tbl");
	}
	node.port = SLAVE_PORT;
	for(i = 0; i < TOTAL_SLAVES; i++){
		if(i == 0)
			sprintf(node.ip, "192.168.56.1");
		else
			sprintf(node.ip, "192.168.56.10%i", i);
		x = write(fd, &node, sizeof(node));
		if(x == -1){
			chk("writing to slave.tbl");
		}
	}
	x = close(fd);
	if(x == -1){
		chk("closing slave.tbl");
	}
	puts("SLAVE TABLE IS RESET");

	/* reseting user.tbl */
	bzero(unode.name, 50);
	bzero(unode.pass, 50);
	strcpy(unode.name, "root");
	strcpy(unode.pass, "root123");
	unode.id = 0;
	fd = open("../server/datalogs/user.tbl", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("opening user.tbl");
	}
	uhead.free = MAX_USERS-2;
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek 0 user.tbl");
	}
	x = write(fd, &uhead, sizeof(uhead));
	if(x == -1){
		chk("writing to user.tbl");
	}
	x = write(fd, &unode, sizeof(unode));
	if(x == -1){
		chk("writing to user.tbl");
	}
	strcpy(unode.name, "nandal");
	strcpy(unode.pass, "");
	unode.id = 1;
	x = write(fd, &unode, sizeof(unode));
	if(x == -1){
		chk("writing to user.tbl");
	}
	x = close(fd);
	if(x == -1){
		chk("closing user.tbl");
	}
	puts("USER TABLE RESET");

	/* reseting group.tbl */
	gnode.id = 0;
	bzero(gnode.name, 50);
	strcpy(gnode.name, "root");
	fd = open("../server/datalogs/group.tbl", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("opening group.tbl");
	}
	ghead.free = MAX_GROUPS-1;
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek 0 group.tbl");
	}
	x = write(fd, &ghead, sizeof(ghead));
	if(x == -1){
		chk("writing to group.tbl");
	}
	x = write(fd, &gnode, sizeof(gnode));
	if(x == -1){
		chk("writing to group.tbl");
	}
	x = close(fd);
	if(x == -1){
		chk("closing group.tbl");
	}
	puts("GROUP TABLE RESET");


	/* reseting data_block.tbl */
	fd = open("../server/datalogs/data_block.tbl", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd == -1){
		chk("opening data_block.tbl");
	}
	dhead.free = TOTAL_SLAVES * SLAVE_CAPACITY;
	dnode.status = AVAILABLE;
	x = lseek(fd, 0, SEEK_SET);
	if(x == -1){
		chk("lseek data_block.tbl");
	}
	x = write(fd, &dhead, sizeof(myDFS_dhead_s));
	if(x == -1){
		chk("writing to data_block.tbl");
	}
	k = 0;
	for(i = 0; i < TOTAL_SLAVES; i++){
		dnode.slave_id = i;
		for(j = 0; j < SLAVE_CAPACITY; j++){
			dnode.id = k++;
			dnode.sub_id = j;
			x = write(fd, &dnode, sizeof(myDFS_dnode_s));
			if(x == -1){
				chk("writing to data_block.tbl");
			}
		}
	}
	x = close(fd);
	if(x == -1){
		chk("closing data_block.tbl");
	}
	puts("DATA BLOCK TABLE RESET");

	/* creating individual dir for each slaves in warehouse*/
	system("rm -r ../server/datawarehouse/*");
	for(i = 0; i < TOTAL_SLAVES; i++){
		sprintf(dirname, "../server/datawarehouse/%u", i);
		x = mkdir(dirname, 0777);
		if(x == -1){
			chk("creating datastore in datawarehouse");
		}
	}

	printf("all individual dir are created for each slave in datawarehouse\n");
	puts("THE WHOLE FILE SYSTEM IS RESET");
	puts("-------------------------------------------------");
	return 0;	
}
