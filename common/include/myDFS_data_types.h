
/* myDFS Data Types to be used through out the whole system */
#include <time.h>
#ifndef myDFS_DATA_TYPES_
#define myDFS_DATA_TYPES_
	typedef unsigned char 	myDFS_uchar;
	typedef signed char 	myDFS_char;
	typedef unsigned short 	myDFS_ushort;
	typedef signed short	myDFS_short;
	typedef unsigned int	myDFS_uint;
	typedef signed int	myDFS_int;
	typedef unsigned long	myDFS_ulong;
	typedef signed long	myDFS_long;
	typedef double		myDFS_double;
	typedef float		myDFS_float;

	typedef myDFS_uint	myDFS_offset;
	typedef myDFS_uint	myDFS_size;
	typedef myDFS_int	myDFS_ssize;
	typedef myDFS_ushort	myDFS_dev;
	typedef myDFS_uchar	myDFS_nlink;
	typedef myDFS_ushort	myDFS_gid;
	typedef myDFS_ushort	myDFS_uid;
	typedef myDFS_ushort	myDFS_mode;
	typedef myDFS_uint	myDFS_inode;
	typedef myDFS_uchar	myDFS_status;
	typedef myDFS_uint	myDFS_blk;
	typedef	myDFS_ushort	myDFS_id;
	typedef myDFS_uint	myDFS_iid;
	typedef	myDFS_ulong	myDFS_lid;
	typedef	time_t		myDFS_time;
	typedef myDFS_int	myDFS_key;
	typedef myDFS_ushort	myDFS_msg;
	typedef myDFS_int	myDFS_ERROR;
	typedef myDFS_int	myDFS_flag;

	/* COMMON #defines n Constants */
	#define MAX_IP		30
	#define SERVER_IP 	"192.168.56.1"
	#define SERVER_PORT 	7345
	#define SLAVE_PORT 	7346

	#define MAX_USERNAME	50
	#define MAX_PASSWORD	50
	#define MAX_GROUPNAME	50
	#define MAX_FILE_NAME	256
	#define MAX_PATH_SIZE	1024 /* no of characters a path contain */
	#define MAX_FILE_SIZE	1024 /* no of BLKSIZ (1GB) */
	#define MAX_NO_FILES	1024 /* 32 bit inode identifier */
	#define MAX_DIR_ENTRY	1024 /* must be same to MAX_FILE_SIZE no of blks*/
	#define MAX_FILE_OPEN	1024 /* MAX FILE CAN BE OPENED IN THE SYSTEM AT ONE TIME */

	#define MAX_FD_PER_USER	256
	#define MAX_FD		1024
	#define MAX_SESSIONS	1024
	

	#define myDFS_BUFSIZ	256
	#define myDFS_BLKSIZ	1024
	#define myDFS_CYCLES	myDFS_BLKSIZ / myDFS_BUFSIZ

	#define myDFS_REG_FILE	1
	#define myDFS_DIR_FILE 	0
	#define myDFS_LNK_FILE	2

	#define myDFS_EOF_ "myDFS_EOF_"

	typedef struct{
		myDFS_char 	ip[MAX_IP];
		myDFS_short 	port;
	}myDFS_node_s;

	typedef struct{
		myDFS_char	name[MAX_FILE_NAME];
		myDFS_size	size;
		myDFS_time	atime;
		myDFS_time	ctime;
		myDFS_time	mtime;
		myDFS_mode	mode;
		myDFS_uid	uid;
		myDFS_gid	gid;
	}myDFS_fhead_s;


	typedef struct{
		myDFS_char	path[MAX_PATH_SIZE];
		myDFS_char	type;
		myDFS_char	depth;
	}myDFS_path_s;
	#define myDFS_FULL_PATH	0
	#define myDFS_REL_PATH	1

#endif
