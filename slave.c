#include "slave.h"

int main(int argc, char **argv)
{
    printf("slave start ...\n");
    //while(1) {};
    //char path[4096]="../testdir/apple.txt";
    //char word[32]="APPLE";
    struct mail_t mail;
    int *mailsize;
    int size=0;
    int sysfs_fd=0;
    sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
    printf("slave's sysfs_fd=%d\n",sysfs_fd);
    unsigned int word_count=0;
    unsigned int *count;
    count = &word_count;
    //    while(1){
    //Searchword(path,word,count);
    //printf("path=%s,word=%s\n",mail.path,word);
    receive_from_fd(sysfs_fd,&mail);
    //  }
    printf("count=%d\n",word_count);
    return 0;
}

void Searchword(char path[],char word[],unsigned int *count)
{
    int i;
    char lower[32];
    for(i=0; i<32; ++i) {
        lower[i]=tolower(word[i]);
    }
    FILE *fin;
    char character;
    char nowword[20];
    memset(nowword,'\0',sizeof(nowword));
    fin = fopen(path,"r");
    //printf("yaya\n");
    while(fscanf(fin,"%c",&character)!=EOF) {
        character = tolower(character);
        if(character== ' ' || character== '\n' || character== '\t'|| character=='.'
                || character==','||character == '!'|| character=='?') {
            if(strcmp(nowword,lower)==0) {
                *count=*count+1;
            }
            memset(nowword,'\0',sizeof(nowword));
            continue;
        } else if(isascii(character)) {
            sprintf(nowword,"%s%c",nowword,character);
            //			printf("%s\n",nowword);
        }
    }
    fclose(fin);
    return;
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
int receive_from_fd(int sysfs_fd, struct mail_t *mail)
{
    /*
     * write something or nothing
     */
    char message[4128];
    char *delim=",";
    memset(message,'\0',sizeof(message));

    int ret_val = read(sysfs_fd,message,4128);
    //    printf("message=%s\n",message);
    if (ret_val == ERR_EMPTY) {
        /*
         * write something or nothing
         */
    } else {
        /*
         * write something or nothing
         */
        printf("message=%s\n",message);    
    }

    /*
     * write something or nothing
     */
}
