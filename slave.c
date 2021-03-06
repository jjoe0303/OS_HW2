#include "slave.h"

int main(int argc, char **argv)
{
	printf("slave start ...\n");
	struct mail_t mail[1024];
	//int *mailsize;
	//int size=0;
	int sysfs_fd=-1;
	unsigned int word_count=0;
	unsigned int *count;
	count = &word_count;
	signals=0;
	struct mail_t nullmail;
	sprintf(nullmail.data.query_word,"%s","");
	nullmail.data.word_count = 0;
	int countsize;
	kill(getpid(),SIGSTOP);
	usleep(100);
	while(1) {
		//Searchword(path,word,count);
		//printf("path=%s,word=%s\n",mail.path,word);
		if(signals==0) {
			receive_from_fd(sysfs_fd,&mail[mailsize]);
			usleep(1000);
			countsize=mailsize;
		}

		if(signals==1) {
			if(mailsize==0 || countsize==0) {
				kill(getpid(),SIGSTOP);
				signals=2;
				continue;
			}
//			for(int j=0; j<countsize; j++) {
			if(countsize-1>=0) {
				Searchword(mail[countsize-1].file_path,mail[countsize-1].data.query_word,count);
			}
			//Searchword(mail[j].file_path,mail[j].data.query_word,count);
			printf("count=%d, path=%s\n",word_count,mail[countsize-1].file_path);
			if(strcmp(mail[countsize-1].file_path,"")!=0 && word_count!=0) {
//				printf("count=%d, path=%s\n",word_count,mail[countsize-1].file_path);
				mail[countsize-1].data.word_count = word_count;
				send_to_fd(sysfs_fd,&mail[countsize-1]);
				word_count=0;
				countsize--;
			}

			else {
				//		usleep(50);
				countsize--;
				//mail[mailsize].data.word_count = 0;
				//		send_to_fd(sysfs_fd,&nullmail);
			}
//			}
			//signals=3;
			//kill(getpid(),SIGSTOP);
		}

		if(signals==3) {
			//usleep(30);
			send_to_fd(sysfs_fd,&nullmail);
		}

		if(signals==2) {
			kill(getpid(),SIGSTOP);
			mailsize=0;
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
		   || character==','||character == '!'|| character=='?' ||character=='('
		   || character == ')' || character == '[' || character == ']') {
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
		if(signals==3) {
			signals=2;
		}
		return 0;
		//kill(getpid(),SIGSTOP);
	} else {
		mailsize=mailsize-1;
	}
	return 0;
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
		usleep(10);
		//        printf("EMPTY\n");
		signals=1;
		return 0;
	} else {
		mailsize=mailsize+1;
		pch=strtok(message,delim);
		while(pch!=NULL) {
			substr[i++]=pch;
			pch=strtok(NULL,delim);
		}
		if(strcmp(message,",")==0) {
			//            printf("null message\n");
			sprintf((*mail).data.query_word,"%s","0");
			sprintf((*mail).file_path,"%s","");
			return 0;
		}
		printf("pid=%d word=%s , path=%s\n",getpid(),substr[0],substr[1]);
		sprintf((*mail).data.query_word,"%s",substr[0]);
		sprintf((*mail).file_path,"%s",substr[1]);
	}
	return 0;
}
