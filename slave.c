#include "slave.h"

int main(int argc, char **argv)
{
	printf("slave start ...\n");
	//while(1) {};
    char path[4096]="../testdir/apple.txt";
    char word[32]="apple";
    int word_count=0;
    int * count;
    count = &word_count;
    printf("path=%s,word=%s\n",path,word);
    Searchword(path,word,count);
    printf("count=%d\n",word_count);
    return 0;
}

void Searchword(char path[],char word[],unsigned int *count)
{
    FILE *fin;
    char character;
    char nowword[20];
    memset(nowword,'\0',sizeof(nowword));
    fin = fopen(path,"r");
    //printf("yaya\n");
    while(fscanf(fin,"%c",&character)!=EOF)
    {
        if(character== ' ' || character== '\n' || character== '\t'){
            memset(nowword,'\0',sizeof(nowword));
            continue;
        }
        else if(isascii(character)){ 
            sprintf(nowword,"%s%c",nowword,character);
            printf("%s\n",nowword);
            if(strcmp(nowword,word)==0){
                *count=*count+1;
            }
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
