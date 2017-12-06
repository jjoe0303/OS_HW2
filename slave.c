#include "slave.h"

int main(int argc, char **argv)
{
	printf("slave start ...\n");
	//while(1) {};
	//char path[4096]="../testdir/apple.txt";
	//char word[32]="APPLE";
	struct mail_t mail[1024];
	//int *mailsize;
	//int size=0;
	int sysfs_fd=-1;
	//sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	//printf("slave's sysfs_fd=%d\n",sysfs_fd);
	unsigned int word_count=0;
	unsigned int *count;
	count = &word_count;
	kill(getpid(),SIGSTOP);
	while(1) {
		//Searchword(path,word,count);
		//printf("path=%s,word=%s\n",mail.path,word);
		if(signals==0) {
			receive_from_fd(sysfs_fd,&mail[mailsize]);
		}

		if(signals==1) {
			if(mailsize-1>=0) {
				Searchword(mail[mailsize-1].file_path,mail[mailsize-1].data.query_word,count);
			}
			//		printf("count=%d, path=%s\n",word_count,mail[mailsize-1].file_path);
			mail[mailsize-1].data.word_count = word_count;
			send_to_fd(sysfs_fd,&mail[mailsize-1]);
			word_count=0;
			//signals=0;
		}

		if(signals==2) {
			kill(getpid(),SIGSTOP);
			signals=0;
		}
	}
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
	char nowword[1000];
	memset(nowword,'\0',sizeof(nowword));
	fin = fopen(path,"r");
	if(fin==NULL) {
		printf("No such file~\n");
		return;
	}
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
//						printf("%s\n",nowword);
		}
	}
	fclose(fin);
	return;
}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	char message[4128];
	sprintf(message,"%u,%s",(*mail).data.word_count,(*mail).file_path);
	//printf("count message:%s\n",message);
	int ret_val =  write(sysfs_fd,message,strlen(message));
	if (ret_val < 0) {
		signals=2;
		kill(getpid(),SIGSTOP);
	} else {
		mailsize=mailsize-1;
	}
}

int receive_from_fd(int sysfs_fd, struct mail_t *mail)
{
	sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	char message[4128];
	char *delim=",";
	char *substr[2];
	char *pch;
	int i =0;

	memset(message,'\0',sizeof(message));
	int ret_val = read(sysfs_fd,message,4128);

	if (ret_val == ERR_EMPTY) {
		kill(getpid(),SIGSTOP);
//        printf("EMPTY\n");
		signals=1;
	} else {
		/*   if(message==NULL){
		       printf("null message!!\n");
		       return 0;
		   }*/
		mailsize=mailsize+1;
		pch=strtok(message,delim);
		while(pch!=NULL) {
			substr[i++]=pch;
			pch=strtok(NULL,delim);
		}
		if(strcmp(message,",")==0) {
//            printf("null message\n");
			return 0;
		}
		//	printf("pid=%d word=%s , path=%s\n",getpid(),substr[0],substr[1]);
		sprintf((*mail).data.query_word,"%s",substr[0]);
		sprintf((*mail).file_path,"%s",substr[1]);
	}
	return 0;
}
