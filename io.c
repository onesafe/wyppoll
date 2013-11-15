#include "wyp.h"


int  Socket(int family, int type, int protocol)
{
	int	sock;
	if((sock = socket(family, type, protocol)) < 0)
	{
		fprintf(stderr, "create socket error[%s][%d]", __FILE__, __LINE__);
		return (EOPENSOCK);
	}
	
	return sock;
}

int  Bind(int sockfd, const struct sockaddr_in myaddr)
{
	if(bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		fprintf(stderr, "bind error[%s][%d]", __FILE__, __LINE__);
		close(sockfd);
		return (EBIND);
	}
}

int  Connect(int sockfd, const struct sockaddr_in servaddr)
{
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		fprintf(stderr, "connect error[%s][%d]", __FILE__, __LINE__);
		close(sockfd);
		return (ECONNECT);
	}
}

int  Listen(int sockfd, int backlog)
{
	if(listen(sockfd, backlog) < 0)
	{
		fprintf(stderr, "listen error[%s][%d]", __FILE__, __LINE__);
		close(sockfd);
		return (ELISTEN);
	}
}


/*
 * 从描述符fd中读取n个字节
 *
 */
ssize_t readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;
	
	ptr = vptr;
	nleft = n;
	
	while(nleft > 0)
	{
		if((nread = read(fd, ptr, nleft)) < 0)
		{
			if(errno == EINTR)  /* call read() again */
				nread = 0;
			else
				return (-1);
		}else if(nread == 0)
		{
			break;		/* EOF */
		}

		nleft -= nread;
		ptr   += nread;
	}

	return (n-nleft);	/* return >= 0 */
}


/*
 * 往文件描述符fd写n个字节
 *
 */
ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t nwritten;
	const char *ptr;
	
	ptr = vptr;
	nleft = n;
	
	while(nleft > 0)
	{
		if((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;	/* call write() again */
			else
				return (-1);	/* error */
		}
		
		nleft -= nwritten;
		ptr   += nwritten;
	}

	return (n);
}


/*
 * 递归建立目录
 *
 * @file 入参 含全路径，文件名
 * 	例：/da/shang/hai
 * 	会建立/da/shang目录
 *
 * 返回@file
 */
char *BuildFile(const char *file)
{
	char *p = NULL;
	char buf[512+19] = {0};
	
	for( p=(char *)file; abs((p=strchr(p, '/'))-file) < strlen(file); p++)
	{
		memset(buf, 0, sizeof(buf));
		memcpy(buf, file, p-file);
		mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}

	return (char *)file;
}



/*
 * 把数字转化成指定长度的buf，位数不足前补0
 *
 * @value 入参
 * @buf   出参
 * @len   入参
 *
 * 返回转化后的buf
 */
char *Int2Buf(int value, char *buf, unsigned int len)
{
	char tmpbuf[64] = {0};
	sprintf(tmpbuf, "%0*d", len, value);
	memcpy(buf, tmpbuf, len);
	buf[len] = '\0';
	return buf;
}


/*
 * 把缓冲区转化成字符串
 *
 * @buf		入参，缓冲区
 * @len		入参，缓冲区长度
 *
 * 返回字符串
 * 注：函数不可重入，缓冲区长度不能大于1024
 */
char *BufToStr(const char *buf, unsigned int len)
{
	static char sBuf[1024 + 1];
	
	memset(sBuf, 0, sizeof(sBuf));
	
	if(len + 1 > sizeof(sBuf))
		return NULL;
	memcpy(sBuf, buf, len);
	sBuf[len] = '\0';
	
	return sBuf;
}

/*
 * 计算当前系统日期时间(YYYYMMDDHHMMSS)
 *
 * @buf		出参 YYYYMMDDHHMMSS
 *
 * 如果buf == NULL，函数不可重入，返回值指向静态变量ret_buf
 * 如果buf != NULL, 函数可重入，返回值指向出参buf
 */
char *DateTimeNow(char *buf)
{
	time_t		now;
	struct tm	*ttime;
	static char	ret_buf[14+1];
	
	memset(ret_buf, 0, sizeof(ret_buf));
	
	time(&now);
	ttime = localtime(&now);
	sprintf(ret_buf, "%4d%02d%02d%02d%02d%02d", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min, ttime->tm_sec);

	if(buf)
	{
		memcpy(buf, ret_buf, 14);
		return buf;
	}

	return ret_buf;
}


/*
 * 计算当前系统日期(YYYYMMDD)
 *
 * @buf		出参 YYYYMMDD
 *
 * 如果buf == NULL, 函数不可重入，返回值指向静态变量ret_buf
 * 如果buf != NULL, 函数可重入，返回值指向出参buf
 */
char *DateNow(char *buf)
{
	static char ret_buf[14+1];
	char		*p;
	
	memset(ret_buf, 0, sizeof(ret_buf));
	p = (buf ? buf : ret_buf);
	memcpy(p, DateTimeNow(NULL), 8);
	p[8] = '\0';

	return p;
}


/*
 * 计算当前系统时间(HHMMSS)
 *
 * @buf		出参 HHMMSS
 *
 * 如果buf == NULL, 函数不可重入，返回值指向静态变量ret_buf
 * 如果buf != NULL, 函数可重入，返回值指向出参buf
 */
char *TimeNow(char *buf)
{
	static char ret_buf[14+1];
	char		*p;
	
	memset(ret_buf, 0, sizeof(ret_buf));
	p = (buf ? buf : ret_buf);
	memcpy(p, DateTimeNow(NULL) + 8, 6);
	p[6] = '\0';

	return p;
}
