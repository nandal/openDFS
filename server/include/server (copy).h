#include "../../common/header/common.h"
#include "../../commonSS/header/commonSS.h"

#ifndef _GLOBAL_SERVER_H_
#define _GLOBAL_SERVER_H_
	extern myDFS_node_s slaves[];
#endif

#define AVAILABLE	0
#define OCCUPIED	1
#define TOTAL_BLOCKS	TOTAL_SLAVES * SLAVE_CAPACITY
#define MAX_USERS	65000
#define MAX_GROUPS	65000

/* for inode table */
typedef struct{
	myDFS_inode 	free;
}myDFS_ihead_s;

typedef struct{
	myDFS_inode 	id;		/* 32 bit identifier */
	myDFS_mode	mode;		/* file type n permissions */
	myDFS_size	size;		/* actual size in bytes */
	myDFS_nlink	nlink;		/* no of hard links */
	myDFS_uid	uid;		/* user's id of owner */
	myDFS_gid	gid;		/* group's id of owner */
	myDFS_blk	blocks; 	/* NO OF BLKSIZ / NO OF FILES FOR DIR */
	myDFS_blk	data[MAX_FILE_SIZE]; /* array pointing to data blocks or file inodes */
	myDFS_time	atime;		/* time of last access */
	myDFS_time	mtime;		/* time of last modification */
	myDFS_time	ctime;		/* time of last status change */
}myDFS_inode_s;

/* data block table */
typedef struct{
	myDFS_blk	free;
}myDFS_dhead_s;

typedef struct{
	myDFS_blk	id;
	myDFS_dev	slave_id;
	myDFS_offset	offset;
	myDFS_status	status;
}myDFS_dnode_s;

/* name to inode(dir) table */
typedef struct{
	myDFS_iid	free;
	myDFS_inode	self;
	myDFS_inode	parent;
}myDFS_nhead_s;

typedef struct{
	myDFS_inode	inode;
	myDFS_char	name[MAX_FILE_NAME];
}myDFS_nnode_s;

/* user table */
typedef struct{
	myDFS_uid	free;
}myDFS_uhead_s;

typedef struct{
	myDFS_uid 	id;
	myDFS_char	name[50];
	myDFS_char	pass[50];
	myDFS_key	key;
}myDFS_unode_s;

/* group table */
typedef struct{
	myDFS_gid 	free;
}myDFS_ghead_s;

typedef struct{
	myDFS_gid	id;
	myDFS_char	name[50];
}myDFS_gnode_s;

/* session details for logins */
typedef struct{
	myDFS_uid	uid;
	myDFS_char	ip[30];
	myDFS_short	port;
	myDFS_time	time;
}myDFS_session_s;

int read_config(myDFS_node_s *);
int first_time_run_only();


int process_request(myDFS_int sockfd, const myDFS_msg_s *);

myDFS_inode name_to_inode(const myDFS_char * fname, myDFS_inode dir);
int make_dir_entry(const myDFS_char *fname, myDFS_inode dir, myDFS_inode inode);
int delete_dir_entry(const myDFS_char *fname, myDFS_inode dir);

myDFS_inode make_inode_entry(myDFS_inode_s *inode_ptr);
int read_inode_entry(myDFS_iid inode_id, myDFS_inode_s *inode_ptr);
int delete_inode_entry(myDFS_iid inode_id);

int get_free_data_blocks(myDFS_blk *data, myDFS_blk n);
int free_data_blocks(myDFS_blk *data, myDFS_blk n);
int read_data_blocks(myDFS_dnode_s *datas, myDFS_blk *data, myDFS_blk n);

myDFS_size read_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

myDFS_size write_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int delete_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int make_dir(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int reset_name_to_inode();

int format(const myDFS_auth_s *auth_ptr);

int stop_slaves(const myDFS_auth_s *auth_ptr);

int handle_slave(myDFS_int sockfd, const myDFS_msg_s *);
int login(myDFS_int sockfd, const myDFS_node_s *node_ptr);

myDFS_size write_file_core(myDFS_int sockfd, const myDFS_inode_s *inode_ptr);
myDFS_size read_file_core(myDFS_int sockfd, const myDFS_inode_s *inode);

int access_permission(const myDFS_auth_s *auth_ptr, const myDFS_msg action_type);
int authorize(const myDFS_auth_s *auth_ptr, const myDFS_msg action_type, myDFS_int nbytes, const myDFS_char *arg);
myDFS_inode get_dir_inode(const myDFS_path_s *cur_ptr, const myDFS_path_s *targ_ptr);

myDFS_int open_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);




