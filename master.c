#include "master.h"

int main(int argc, char **argv)
{
	int ch;
	char ec;
	char word[32];
	char dirname[2048];
	char slaves[1024];
	sprintf(slaves,"%s","2");
	while((ch = getopt(argc, argv, ":q:d:s:")) != -1) {
		switch(ch) {
		case 'q':
			//word = optarg;
			sprintf(word,"%s",optarg);
			printf("Query_word=%s\n",word);
			break;
		case 'd':
			//dirname = optarg;
			sprintf(dirname,"%s",optarg);
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
	int num = atoi(slaves);
	int pid[num];
	int i;
	char k[20];
	struct  mail_t mail[1024];
	int *mailsize;
	int size = 0;
	mailsize = &size;
	ConstructMail(dirname,mail,word,mailsize);
	printf("size=%d\n",size);
	for(i=0; i<size; i++) {
		printf("%d:\n",i);
		printf("\t%s\n",mail[i].data.query_word);
		printf("\t%s\n",mail[i].file_path);
	}
	for(i=0; i<num; ++i) {
		pid[i]=fork();
		if(pid[i]==0) {
			execl("./slave","slave","executed by execl",NULL);
		}
	}
	while(1) {
		scanf("%s",&k[0]);
		if(strcmp(k,"kill")==0) {
			for(i=0; i<num; ++i) {
				kill(pid[i],SIGTERM);
			}
		}
	}

	return 0;

}

void ConstructMail(char dirname[],struct mail_t mail[],char word[],
                   int *mailsize)
{

	/*open directory*/
	DIR * dir = opendir(dirname);
	struct dirent *filename;
	char subdir[2048];
	//char base[1000];
	//memset(base,'\0',sizeof(base));
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
			printf("mailsize=%d , filepath=%s\n",*mailsize,mail[*mailsize].file_path);
			sprintf(mail[*mailsize].data.query_word,"%s",word);
			*mailsize=*mailsize+1;
		} else if(filename->d_type == 4) {
			//memset(base,'\0',sizeof(base));
			sprintf(subdir,"%s",dirname);
			sprintf(subdir,"%s%s/",subdir,filename->d_name);
			printf("subdir=%s\n",subdir);
			ConstructMail(subdir,mail,word,mailsize);
			// printf("%s\n",base);
		}
	}
	//  printf("%s\n",base);
	closedir(dir);


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
