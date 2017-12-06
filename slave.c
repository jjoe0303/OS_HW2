#include "slave.h"

int main(int argc, char **argv)
{
	printf("slave start ...\n");
	//while(1) {};
	//char path[4096]="../testdir/apple.txt";
	//char word[32]="APPLE";
	struct mail_t mail[100];
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
			Searchword(mail.file_path,mail.data.query_word,count);
			printf("count=%d\n",word_count);
			word_count=0;
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

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{

	sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	char message[4128];
	int ret_val =  write(sysfs_fd,message,strlen(message));
	if (ret_val == ERR_FULL) {
		kill(getpid(),SIGSTOP);
		signals=2;
	} else {
		mailsize=mailsize-1;
	}
}

int receive_from_fd(int sysfs_fd, struct mail_t *mail)
{
	/*
	 * write something or nothing
	 */
	sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	char message[4128];
	char *delim=",";
	char *substr[2];
	char *pch;
	int i =0;

	memset(message,'\0',sizeof(message));
	int ret_val = read(sysfs_fd,message,4128);

	if (ret_val == ERR_EMPTY) {
		/*
		 * write something or nothing
		 */
		kill(getpid(),SIGSTOP);
		signals=1;
	} else {
		/*
		 * write something or nothing
		 */
		mailsize=mailsize+1;
		pch=strtok(message,delim);
		while(pch!=NULL) {
			substr[i++]=pch;
			//printf("pch=%s\n",pch);
			pch=strtok(NULL,delim);
		}
		printf("word=%s , path=%s\n",substr[0],substr[1]);
		sprintf((*mail).data.query_word,"%s",substr[0]);
		sprintf((*mail).file_path,"%s",substr[1]);
		// signals=1;
//        kill(getpid(),STOP);
	}
	/*
	 * write something or nothing
	 */
	return 0;
}
