#include <sys/types.h>
#include <sys/stat.h>
#include <sys/klog.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd,i=0;
	char buf[10] ="0123456789";
	char *p;
	p=buf;
	fd = open("/dev/cdevdemo",O_RDWR);		//打开文件
	if(fd<0)
	{
		perror("open fail \n");
		return 1;
	}
	while(1)
	{
		write(fd,p,strlen(p));			//写入所设置内容
		printf("%d   : %s \n", i,p);
		i++;
		p++;
		if(i%10==0)
		{
			p=buf;
		}
	//	usleep(500000);					//延时
	}
	return 0;
}