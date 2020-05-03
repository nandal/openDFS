#include "../../common/include/common.h"
#include "../../commonSS/include/commonSS.h"

#include "/HD/storage/project/myDFS/common/include/myDFS_msg.h"

#define SLAVE_WAREHOUSE "warehouse"

/* slave will read config file and configure itself to serve the master */
int slave_read_config(myDFS_node_s *);

/* this function will makes its entry in slave list on server end */
int slave_make_entry_slave_list(const myDFS_node_s *);

/* slave_read_data will read data from data store n write to sockfd */
myDFS_size slave_read_data(myDFS_int sockfd);

/* slave_write_data will read data from sockfd and write to data store */
myDFS_size slave_write_data(myDFS_int sockfd);

/* this function will process the request from the master */
int slave_process_request(myDFS_int sockfd, const myDFS_msg);



