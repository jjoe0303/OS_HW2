#include "master.h"

int main(int argc, char **argv){
    int ch;                      
    char ec;                              
    char *word;
    char *dirname;
    char slaves[4096];
    sprintf(slaves,"%s","2");
    while((ch = getopt(argc, argv, ":q:d:s:")) != -1)
    {
        switch(ch)
        {
            case 'q':
                word = optarg;
                printf("Query_word=%s\n",word);
                break;
            case 'd':
                dirname = optarg;
                printf("Directory=%s\n",dirname);
                break;
            case 's':
                sprintf(slaves,"%s",optarg); 
                break;
            case '?':
                ec = (char)optopt;
                printf("Invalid argument\'%c\'!\n", ec);
                break;
            case ':':
                printf("lack of parameter!\n");
                break;
        }
    }
    printf("Num of slaves=%s\n", slaves);
    return 0;

}

//int send_to_fd(int sysfs_fd, struct mail_t *mail)
//{
//    /*
//     * write something or nothing
//     */
//
//    int ret_val = write(sysfs_fd, ...);
//    if (ret_val == ERR_FULL) {
//        /*
//         * write something or nothing
//         */
//    } else {
//        /*
//         * write something or nothing
//         */
//    }
//
//    /*
//     * write something or nothing
//     */
//}
//
//int receive_from_fd(int sysfs_fd, struct mail_t *mail)
//{
//    /*
//     * write something or nothing
//     */
//
//    int ret_val = read(sysfs_fd, ...);
//    if (ret_val == ERR_EMPTY) {
//        /*
//         * write something or nothing
//         */
//    } else {
//        /*
//         * write something or nothing
//         */
//    }
//
//    /*
//     * write something or nothing
//     */
//}
