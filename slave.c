#include "slave.h"

int main(int argc, char **argv){
    printf("slave start ...\n");
    while(1){};
    return 0;
}

//int send_to_fd(int sysfs_fd, struct mail_t *mail)
//{
//	/*
//	 * write something or nothing
//	 */
//
//	int ret_val = write(sysfs_fd, ...);
//	if (ret_val == ERR_FULL) {
//		/*
//		 * write something or nothing
//		 */
//	} else {
//		/*
//		 * write something or nothing
//		 */
//	}
//
//	/*
//	 * write something or nothing
//	 */
//}
//
//int receive_from_fd(int sysfs_fd, struct mail_t *mail)
//{
//	/*
//	 * write something or nothing
//	 */
//
//	int ret_val = read(sysfs_fd, ...);
//	if (ret_val == ERR_EMPTY) {
//		/*
//		 * write something or nothing
//		 */
//	} else {
//		/*
//		 * write something or nothing
//		 */
//	}
//
//	/*
//	 * write something or nothing
//	 */
//}
