#include <sys/types.h>
#include <sys/stat.h>
#include <sys/klog.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
	char temp[512]={0};
	int i=1,fd_log,buf_size=0,buf_flag=0;
	int size_buf=0;
	struct stat st;
	fd_log = open("./loginfo.log", O_WRONLY | O_CREAT, 0640);	//打开需要写入的日志文件
	FILE *fd_set = fopen("./Offset.txt", "rt");		//打开存放偏移量的文件
	if(fd_log < 0)
	{
		perror("open loginfo.log fail\n");
	}
	if(fd_set == 0)		//文件不存在，重新创建文件，并写入0
	{
		printf("Create the Offset.txt\n");
		fd_set = fopen("./Offset.txt","wt");
		fprintf(fd_set,"%d",0);
	}
	
	fscanf(fd_set,"%d",&buf_size);		//获取偏移量
	lseek(fd_log,buf_size,SEEK_SET);	//指向上次结束时的位置
	
	while(1)
	{
		if(buf_size > 1024*1024)		//检测是否超过所定大小
		{
			buf_flag=1;
			buf_size=0;
		}
		if(buf_flag)			//跳转到文件头
		{
			printf("1111\n");
			lseek(fd_log,0,SEEK_SET);
			buf_flag=0;
		}
		
		klogctl(4, temp, 512);				//获取日志文件内容
		printf("%d :\n%s\n", i,temp);		//打印内容
		write(fd_log,temp,strlen(temp));	//将内容写入到自定义文件中
		buf_size +=strlen(temp);			//增加偏移量
		fd_set=NULL;						//清空.txt文件内容
		fd_set = fopen("./Offset.txt", "w");
		fprintf(fd_set,"%d",buf_size);		//写入新的偏移量值
		fclose(fd_set);
		printf("new fd_size %d\n",buf_size);
		i++;
	//	sleep(1);
		usleep(500000);	
	}
	close(fd_log);
	return 0;
} 