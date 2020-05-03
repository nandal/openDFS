#include "../dfs_client/include/dfs_client.h"

myDFS_size write_file(const myDFS_char *arg);

myDFS_size read_file(const myDFS_char *arg);

int delete_file(const myDFS_char *arg);

int list_file(const myDFS_char *arg);

int ask_server_stop();

void help();

int process(const myDFS_char *choice);
int move_file(const myDFS_char *arg);
int copy_file(const myDFS_char *arg);
int change_dir(const myDFS_char *arg);
int make_dir(const myDFS_char *arg);
int create_path(myDFS_path_s *path_ptr, const myDFS_char *path);
myDFS_char * get_filename(const myDFS_char *path);

myDFS_int dfs_open(const myDFS_char *path, myDFS_int flags, myDFS_mode mode);
myDFS_int dfs_close(myDFS_int fd);
myDFS_ssize dfs_write(myDFS_int fd, const myDFS_char *buf, myDFS_ssize len);
myDFS_ssize dfs_read(myDFS_int fd, myDFS_char *buf, myDFS_ssize len);


