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

