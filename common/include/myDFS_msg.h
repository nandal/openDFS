#include "myDFS_data_types.h"


#define myDFS_MSG_SUCCESS	1
#define myDFS_MSG_FAILURE	2

#define myDFS_MSG_OPEN		3
#define myDFS_MSG_CLOSE		4
#define myDFS_MSG_WRITE		5
#define myDFS_MSG_READ		6
#define myDFS_MSG_CREAT		7
#define myDFS_MSG_CHDIR		8
#define myDFS_MSG_CHMOD		9
#define myDFS_MSG_CHOWN		10
#define myDFS_MSG_LINK		11
#define myDFS_MSG_MKDIR		12
#define myDFS_MSG_RMDIR		13
#define myDFS_MSG_UNLINK	14
#define myDFS_MSG_GETCWD	15
#define myDFS_MSG_LSEEK		16

#define myDFS_MSG_LOGIN		101
#define myDFS_MSG_LOGOUT	102

#define myDFS_MSG_STOP		103
#define myDFS_MSG_FORMAT	104
#define myDFS_MSG_STATUS	105


//#define myDFS_MSG_UPDATE_USR	14
//#define myDFS_MSG_ADD_USR	16
//#define myDFS_MSG_DEL_USR	17
//#define myDFS_MSG_STORAGE	18

typedef struct{
	myDFS_uid	uid;
	myDFS_key	key;
}myDFS_auth_s;

/* login() */
typedef struct{
	myDFS_char	name[MAX_USERNAME];
	myDFS_char 	pass[MAX_PASSWORD];
}myDFS_login_s;

/* VARIOUS ARGUMENTS FOR ALL SYS CALLS */
/* open() */
typedef struct{
	myDFS_path_s	path;
	myDFS_int	flags;
	myDFS_mode	mode;
}myDFS_OPEN_ARG;

/* read() & write() */
typedef struct{
	myDFS_int	fd;
	myDFS_ssize	len;
}myDFS_RW_ARG;

/* lseek() */
typedef struct{
	myDFS_int 	fd;
	myDFS_offset	offset;
	myDFS_int	ref;
}myDFS_LSEEK_ARG;

/* creat(), chmod() & mkdir() unlink()*/
typedef struct{
	myDFS_path_s	path;
	myDFS_mode	mode;
}myDFS_CREAT_ARG;

/* chown() */
typedef struct{
	myDFS_path_s	path;
	myDFS_uid	uid;
	myDFS_gid	gid;
}myDFS_CHOWN_ARG;

/* link() */
typedef struct{
	myDFS_path_s	old;
	myDFS_path_s	new;
}myDFS_LINK_ARG;


/* File MODE related MACROS n #defines */
#define	myDFS_MODE_XO	1
#define	myDFS_MODE_WO	2
#define	myDFS_MODE_RO	4
#define	myDFS_MODE_XG	8
#define	myDFS_MODE_WG	16
#define	myDFS_MODE_RG	32
#define	myDFS_MODE_XU	64
#define	myDFS_MODE_WU	128
#define	myDFS_MODE_RU	256
#define	myDFS_MODE_REG	512
#define	myDFS_MODE_DIR	1024
#define myDFS_MODE_FLAG	65024

#define myDFS_IS_DIR(MODE)	MODE & myDFS_MODE_DIR
#define myDFS_IS_REG(MODE)	MODE & myDFS_MODE_REG

/* Flags for opening files */
#define myDFS_O_RDONLY	1
#define myDFS_O_WRONLY	2
#define myDFS_O_RDWR	4
#define myDFS_O_TRUNC	8
#define myDFS_O_CREAT	16
#define myDFS_O_APPEND	32

/* lseek reference flag */
#define myDFS_SEEK_CUR	0
#define myDFS_SEEK_SET	1
#define myDFS_SEEK_END	2













