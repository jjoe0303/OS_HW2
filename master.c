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
			//word = optarg;
			sprintf(word,"%s",optarg);
//			printf("Query_word=%s\n",word);
			break;
		case 'd':
			//dirname = optarg;
			sprintf(dirname,"%s",optarg);
			if(dirname[strlen(dirname)-1] != '/') {
				sprintf(dirname,"%s%c",dirname,'/');
			}
//			printf("Directory=%s\n",dirname);
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
//	printf("Num of slaves=%s\n", slaves);
	int num = atoi(slaves);
	int pid[num];
	int i;
//	char k[20];
	struct  mail_t mail[1024];
	// struct  mail_t buffer[1024];
	int *mailsize;
	int size = 0;
	int sysfs_fd=0;
	int sid[num];
	int *status;
	mailsize = &size;
	totalcount=0;
	ConstructMail(dirname,mail,word,mailsize);
//	printf("size=%d\n",size);
//	for(i=0; i<size; i++) {
//		printf("%d:\n",i);
//		printf("\t%s\n",mail[i].data.query_word);
//		printf("\t%s\n",mail[i].file_path);
//	}
	for(i=0; i<num; ++i) {
		pid[i]=fork();
		if(pid[i]==0) {
			sid[i]=getpid();
//			printf("slave pid[%d]=%d , my parentid=%d\n",i,getpid(),getppid());
			execl("./slave","slave","executed by execl",NULL);
		}
		// printf("slave pid[%d]=%d\n",i,getpid());
	}
	realsize=0;
	//	sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	//i=0;
//	scanf("%s",&k[0]);
	int key=0;
	// sleep(2);
	while(!key) {
		//scanf("%s",&k[0]);
		if(signals==0) { //send stage
//			printf("Send message...\n");
			send_to_fd(sysfs_fd,&mail[realsize]);
			usleep(1); //here need to sleep
		}

		if(signals==1) {
//			printf("Wakeup slave...");
			for(i=0; i<num; ++i) {
				kill(sid[i],SIGCONT);
				//printf("haha\n");
			}
			// sleep(1);
//			printf("OK\n");

//			printf("wait for slave read...");
			for(i=0; i<num; ++i) {
				waitpid(sid[i],WIFSTOPPED(status),WUNTRACED);
			}
			// sleep(2);
//			printf("ok\n");

//			printf("Wakeup slave...");
			for(i=0; i<num; ++i) {
				kill(sid[i],SIGCONT);
			}
//			printf("ok\n");

//			printf("wait for slave write...");
			for(i=0; i<num; ++i) {
				waitpid(sid[i],WIFSTOPPED(status),WUNTRACED);
			}
			//sleep(2);
//			printf("ok\n");
			signals=2;
		}

		if(signals==2) { //read stage
			receive_from_fd(sysfs_fd,&mail[0]);
		}

		if(signals==3) { //read stage
			for(i=0; i<num; ++i) {
				kill(sid[i],SIGCONT);
			}
			signals=0;
//			printf("realsize=%d\n",realsize);
			if(realsize>=size) key=1;
		}


//		if(strcmp(k,"kill")==0) {
//			for(i=0; i<num; ++i) {
//				kill(pid[i],SIGTERM);
//			}
//			printf("Killed!!\n");
//		}
		//	sleep(1);  //here cause error!!
	}
	sleep(3);
	for(i=0; i<num; ++i) {
		kill(pid[i],SIGTERM);
	}
//	printf("Killed!!\n");
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

	// int mailsize=0;
	while((filename=readdir(dir)) != NULL) {
		if(strcmp(filename->d_name,".")==0 || strcmp(filename->d_name,"..")==0)
			continue;
		else if(filename->d_type != 4) {
			sprintf(mail[*mailsize].file_path,"%s%s",dirname,filename->d_name);
			//		printf("mailsize=%d , filepath=%s\n",*mailsize,mail[*mailsize].file_path);
			sprintf(mail[*mailsize].data.query_word,"%s",word);
			*mailsize=*mailsize+1;
		} else if(filename->d_type == 4) {
			//memset(base,'\0',sizeof(base));
			sprintf(subdir,"%s",dirname);
			sprintf(subdir,"%s%s/",subdir,filename->d_name);
			//		printf("subdir=%s\n",subdir);
			ConstructMail(subdir,mail,word,mailsize);
			// printf("%s\n",base);
		}
	}
	//  printf("%s\n",base);
	closedir(dir);


}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	/*
	 * write something or nothing
	 */
	//	printf("sysfs_fd=%d\n",sysfs_fd);
	sysfs_fd=open("/sys/kernel/hw2/mailbox",O_RDWR);
	char message[4128];
	sprintf(message,"%s,%s",(*mail).data.query_word,(*mail).file_path);

	//	char *d= "hello world!";
	//	int ret_val = write(sysfs_fd,d,strlen(d));
	int ret_val = write(sysfs_fd,message,strlen(message));
	//printf("ret_val=%d\n",ret_val);
	if (ret_val < 0) {
		/*
		 * write something or nothing
		 */
		//        send_to_fd(sysfs_fd,mail);
		//printf("haha\n");
		signals=1;

	} else {
		/*
		 * write something or nothing
		 */
		realsize=realsize+1;
	}

	/*
	 * write something or nothing
	 */
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
	int i=0;
	memset(message,'\0',sizeof(message));

	int ret_val = read(sysfs_fd,message,4128);
	if (ret_val == ERR_EMPTY) {
		signals=3;
	} else {
//		printf("master receive:%s\n",message);
		pch=strtok(message,delim);
		while(pch!=NULL) {
			substr[i++]=pch;
			pch=strtok(NULL,delim);
		}
		unsigned int localcount=atoi(substr[0]);
//        sleep(1);
		totalcount=totalcount+localcount;
	}
	return 0;

}
