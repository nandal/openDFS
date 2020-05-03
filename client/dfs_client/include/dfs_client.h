#include "/HD/storage/project/myDFS/common/include/common.h"
#include "/HD/storage/project/myDFS/common/include/myDFS_msg.h"
#include "/HD/storage/project/myDFS/common/include/myDFS_error.h"

#ifndef _GLOBAL_CLIENT_H_
#define _GLOBAL_CLIENT_H_
	extern	myDFS_char	user[MAX_USERNAME];
	extern 	myDFS_auth_s	auth;		/* used as authrization certificate of the user */
	extern	myDFS_char	server_ip[MAX_IP];
	extern	myDFS_short	server_port;
	extern 	myDFS_path_s	current_dir;
	extern 	myDFS_ERROR	ERROR;
#endif


myDFS_int dfs_open(const myDFS_char *path, myDFS_int flags, myDFS_mode mode);
myDFS_int dfs_close(myDFS_int fd);
myDFS_ssize dfs_write(myDFS_int fd, const myDFS_char *buf, myDFS_int len);
myDFS_ssize dfs_read(myDFS_int fd, myDFS_char *buf, myDFS_int len);
myDFS_int dfs_creat(const myDFS_char *path, myDFS_mode mode);
myDFS_offset dfs_lseek(myDFS_int fd, myDFS_offset offset, myDFS_int ref);

myDFS_int dfs_chdir(const myDFS_char *path);
myDFS_int dfs_chmod(const myDFS_char *path, myDFS_mode mode);
myDFS_int dfs_chown(const myDFS_char *path, myDFS_uid owner, myDFS_gid group);
myDFS_int dfs_link(const myDFS_char *oldpath, const myDFS_char *newpath);
myDFS_int dfs_mkdir(const myDFS_char *path, myDFS_mode mode);
myDFS_int dfs_rmdir(const myDFS_char *path);
myDFS_int dfs_unlink(const myDFS_char *path);
myDFS_char *dfs_getcwd(myDFS_char *buf, myDFS_size size);

myDFS_int dfs_login(const myDFS_login_s *login);
myDFS_int dfs_logout();




