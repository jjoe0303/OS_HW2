#include "master.h"

int main(int argc, char **argv)
{
	int ch;
	char ec;
	char word[32];
	char dirname[4096];
	char slaves[1024];
	sprintf(slaves,"%s","2");
	while((ch = getopt(argc, argv, ":q:d:s:")) != -1) {
		switch(ch) {
		case 'q':
			sprintf(word,"%s",optarg);
			break;
		case 'd':
			sprintf(dirname,"%s",optarg);
			if(dirname[strlen(dirname)-1] != '/') {
				sprintf(dirname,"%s%c",dirname,'/');
			}
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
	int num = atoi(slaves);
	int pid[num];
	int i;
	struct  mail_t mail[1024];
	int *mailsize;
	int size = 0;
	int sysfs_fd=0;
	int *status;
	mailsize = &size;
	ConstructMail(dirname,mail,word,mailsize);
	for(i=0; i<num; ++i) {
		pid[i]=fork();
		if(pid[i]==0) {
			execl("./slave","slave","executed by execl",NULL);
		}
	}
	realsize=0;
	int key=0;
	signals=0;
	while(!key) {
		if(signals==0) { //send stage
			send_to_fd(sysfs_fd,&mail[realsize]);
		}

		if(signals==1) {
			for(i=0; i<num; ++i) {
				kill(pid[i],SIGCONT);
			}
			for(i=0; i<num; ++i) {
				waitpid(pid[i],WIFSTOPPED(status),WUNTRACED);
			}
			for(i=0; i<num; ++i) {
				kill(pid[i],SIGCONT);
			}
			for(i=0; i<num; ++i) {
				waitpid(pid[i],WIFSTOPPED(status),WUNTRACED);
			}
			signals=2;
		}

		if(signals==2) { //read stage
			receive_from_fd(sysfs_fd,&mail[0]);
		}

		if(signals==3) { //read stage
			for(i=0; i<num; ++i) {
				kill(pid[i],SIGCONT);
			}
			signals=0;
			if(realsize>=size) key=1;
		}
		usleep(1);  //here cause error!!
	}
	for(i=0; i<num; ++i) {
		kill(pid[i],SIGTERM);
	}
	printf("The total number of query word \"%s\" is %u\n",word,totalcount);
	return 0;
}

void ConstructMail(char dirname[],struct mail_t mail[],char word[],
                   int *mailsize)
{
	/*open directory*/
	DIR * dir = opendir(dirname);
	struct dirent *filename;
	char subdir[2048];
	if((dir=opendir(dirname)) == NULL) {
		perror("Open dir error...");
		exit(1);
	}

	while((filename=readdir(dir)) != NULL) {
		if(strcmp(filename->d_name,".")==0 || strcmp(filename->d_name,"..")==0)
			continue;
		else if(filename->d_type != 4) {
			sprintf(mail[*mailsize].file_path,"%s%s",dirname,filename->d_name);
			//		printf("mailsize=%d , filepath=%s\n",*mailsize,mail[*mailsize].file_path);
			sprintf(mail[*mailsize].data.query_word,"%s",word);
			*mailsize=*mailsize+1;
		} else if(filename->d_type == 4) {
			sprintf(subdir,"%s",dirname);
			sprintf(subdir,"%s%s/",subdir,filename->d_name);
			//		printf("subdir=%s\n",subdir);
			ConstructMail(subdir,mail,word,mailsize);
		}
	}
	closedir(dir);
}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	//	printf("sysfs_fd=%d\n",sysfs_fd);
	sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	char message[4128];
	sprintf(message,"%s,%s",(*mail).data.query_word,(*mail).file_path);

	//	char *d= "hello world!";
	//	int ret_val = write(sysfs_fd,d,strlen(d));
	int ret_val = write(sysfs_fd,message,strlen(message));
	//printf("ret_val=%d\n",ret_val);
	if (ret_val < 0) {
		//        send_to_fd(sysfs_fd,mail);
		//printf("haha\n");
		signals=1;

	} else {
		realsize=realsize+1;
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
	int i=0;
	memset(message,'\0',sizeof(message));

	int ret_val = read(sysfs_fd,message,4128);
	if (ret_val == ERR_EMPTY) {
		signals=3;
	} else {
		pch=strtok(message,delim);
		while(pch!=NULL) {
			substr[i++]=pch;
			pch=strtok(NULL,delim);
		}
		unsigned int localcount=atoi(substr[0]);
		totalcount=totalcount+localcount;
		printf("localcount=%u , totalcount=%u\n",localcount,totalcount);
	}
	return 0;

}
