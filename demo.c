#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include <time.h>
#include "common/header/common.h"


int main(){
	myDFS_time t;
	time(&t);
	printf("%u is long value of time\n", t);	
	printf("Time: %s", ctime(&t));
	return 0;
}
