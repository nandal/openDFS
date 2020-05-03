#include "myDFS_data_types.h"
#define	USER_TBL ""
#define GROUP_TBL ""
#define LOGIN_TBL ""
#define INODE_TBL ""
#define DATA_BLOCK_TBL ""
#define DIR_FOLDER ""
#define SESSION_DIR ""
#define SERVER_DATALOG "/HD/storage/project/myDFS/server/datalogs"
#define SERVER_WAREHOUSE "/HD/storage/project/myDFS/server/datawarehouse"
#define CLIENT_DATALOG ""
#define SLAVE_DATALOG ""


/* a macro function for inserting the debugging code in compilation */
#ifndef __NO_DEBUG__
#define chk(reason) perror(reason);\
printf("%s:%d\n", __FILE__,__LINE__);
#else
#define chk(reason) 
#endif


