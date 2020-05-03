#include "include/client.h"

void help(){
	puts("commands\t\tuses");
	puts("ls [dir]\t\tto list the files of local machine");
	puts("lsr [dir]\t\tto list the files or remote machine");
	puts("cd [dir]\t\tto change the directory of local machine");
	puts("cdr dir]\t\tto change the directory of remote machine");
	puts("cat <file>\t\tto copy the file to local machine");
	puts("put <file>\t\tto copy the file to server machine");
	puts("mkdir <directory>\tto create a directory on local machine");
	puts("mkdirr <directory>\tto create a directory on server");
	puts("lnr <old path> <new path>\tto create a link on server");
	puts("chmodr <path> <mode>\tto change mode server");
	puts("chownr <path> <uid> <gid>\tto change ownership on server");
	puts("rmr <file>\t\tto delete or remove the remote file");
	puts("rm <file>\t\tto delete or remove the local file");
	puts("mv <src> <dest>\t\tto rename or move the remote file");
	puts("pwdr\t\t\tto list the current full path of local directory");
	puts("pwd\t\t\tto list the current full path of remote directory");
	puts("clear\t\t\tto to clean the screen");
	puts("exit\t\t\tto return");
	puts("[...] are optional arguments");	
	puts("<...> are compulsory arguments");
}
