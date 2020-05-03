#include "/HD/storage/project/myDFS/common/include/common.h"
#include "/HD/storage/project/myDFS/commonSS/include/commonSS.h"
#include "/HD/storage/project/myDFS/common/include/myDFS_msg.h"
#include "/HD/storage/project/myDFS/common/include/myDFS_error.h"

#define AVAILABLE	0
#define OCCUPIED	1
#define TOTAL_BLOCKS	TOTAL_SLAVES * SLAVE_CAPACITY
#define MAX_USERS	65000
#define MAX_GROUPS	65000

#define myDFS_UNLOCKED	0
#define myDFS_LOCKED	1

/* for global meta info at server end */
typedef struct{
	myDFS_uid	total_logged;
	myDFS_uid	free_logged;
	myDFS_int	total_fd;
	myDFS_int	free_fd;
	myDFS_int	total_ft;
	myDFS_int	free_ft;
	myDFS_inode	total_inode;
	myDFS_inode	free_inode;
}myDFS_server_status_s;

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
	myDFS_char	lock;
}myDFS_inode_s;

/* data block table */
typedef struct{
	myDFS_blk	free;
}myDFS_dhead_s;

typedef struct{
	myDFS_blk	id;
	myDFS_dev	slave_id;
	myDFS_uint	sub_id;
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
	myDFS_char	name[MAX_USERNAME];
	myDFS_char	pass[MAX_PASSWORD];
}myDFS_unode_s;

/* group table */
typedef struct{
	myDFS_gid 	free;
}myDFS_ghead_s;

typedef struct{
	myDFS_gid	id;
	myDFS_char	name[MAX_GROUPNAME];
}myDFS_gnode_s;

/* session details for logins */
typedef struct{
	myDFS_uid	uid;
	myDFS_char	ip[MAX_IP];
	myDFS_short	port;
	myDFS_time	time;
}myDFS_session_s;

/* data structures for user session management */
/* open fd table, uid n key details, + IP details */

typedef struct{
	myDFS_char	lock;
	myDFS_flag	flags;
	myDFS_inode	inode;
	myDFS_int	count;
}myDFS_ftnode_s;

/* file descriptor table */
typedef struct{
	myDFS_offset	offset;
	myDFS_flag	flags;
	myDFS_int	ftid;	/* file table id */	
}myDFS_fdnode_s;

typedef struct{
	myDFS_path_s	current;
	myDFS_key	key;
	myDFS_char	ip[MAX_IP];
	myDFS_int	total_fd;
	myDFS_int	fd_index[MAX_FD_PER_USER];
}myDFS_usnode_s;

int read_config(myDFS_node_s *);

int initialize_globals();

int login_file(myDFS_int sockfd, const myDFS_node_s *node_ptr);

int logout_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int process_request(myDFS_int sockfd, const myDFS_msg msg);

int open_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int close_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int read_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int write_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int creat_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int chdir_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int chmod_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int chown_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int link_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int mkdir_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int rmdir_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int unlink_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int stop_slaves(const myDFS_auth_s *auth_ptr);

int format(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

int status_file(myDFS_int sockfd, const myDFS_auth_s *auth_ptr);

/* helping functions */
int lock_inode(myDFS_inode inode);
int unlock_inode(myDFS_inode inode);
int create_path(myDFS_path_s *path_ptr, const myDFS_char *path);
int access_permission(const myDFS_auth_s *auth_ptr, const myDFS_msg msg);
int authorize(const myDFS_auth_s *auth_ptr, const myDFS_int flags, const myDFS_inode id);
myDFS_inode get_inode_from_path(const myDFS_path_s *path_ptr);
myDFS_inode get_new_inode_using_path(const myDFS_path_s *path_ptr, myDFS_inode_s *inode_ptr, myDFS_inode *parent_ptr);
myDFS_inode name_to_inode(const myDFS_char *fname, myDFS_inode dir_id);
myDFS_int get_fd(const myDFS_auth_s *auth_ptr, const myDFS_inode inode, myDFS_int flags);
myDFS_int close_fd(const myDFS_auth_s *auth_ptr, const myDFS_int fd);
myDFS_inode make_inode_entry(myDFS_inode_s *inode_ptr);
myDFS_int update_inode_entry(myDFS_inode_s *inode_ptr);
myDFS_int make_dir_entry(const myDFS_char *fname, myDFS_inode dir_id, myDFS_inode file_id);
myDFS_int read_inode_entry(myDFS_inode inode_id, myDFS_inode_s *inode_ptr);
myDFS_int get_free_data_blocks(myDFS_blk *data, myDFS_blk n);

myDFS_int free_data_block(const myDFS_blk data_id);
myDFS_blk get_free_data_block(myDFS_dnode_s *dnode_ptr);

myDFS_int write_block(const myDFS_dnode_s * dnode_ptr);
myDFS_int get_block(const myDFS_dnode_s	* dnode_ptr);

myDFS_int read_data_block_entry(myDFS_dnode_s *dnode_ptr, myDFS_blk block_id);


#ifndef _GLOBAL_SERVER_H_
#define _GLOBAL_SERVER_H_
	extern myDFS_node_s 	slaves[];
	extern myDFS_ERROR	ERROR;

	/* session related globals */
	extern myDFS_usnode_s		*logged_pa[];
	extern myDFS_server_status_s	comp_meta;
	extern myDFS_fdnode_s		*fd_global_pa[];
	extern myDFS_ftnode_s		*ft_global_pa[];
	extern myDFS_inode_s 		*inode_global_pa[];
#endif

