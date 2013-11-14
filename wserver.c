#include "wyp.h"

/******************************************/
/*********** author: onesafe **************/
/*********** 2013.11.14      **************/
/******************************************/


/****** 函数声明 ******/
extern int  Socket(int family, int type, int protocol);
extern int  Bind(int sockfd, const struct sockaddr_in myaddr);
extern int  Listen(int sockfd, int backlog);
extern ssize_t writen(int fd, const void *vptr, size_t n);

int init_socket_server(int iPort, int backlog);
void str_echo(int sockfd);


int main(void)
{
	int	sock, connfd;
	int	child_pid;
	struct sockaddr_in cliaddr;
	size_t cliaddrlen;

	sock = init_socket_server(9999, 60);
	if(sock < 0)
	{
		fprintf(stderr, "初始化网络服务错误[%s][%d]\n", __FILE__, __LINE__);
		exit(-1);
	}
printf("all ready,waiting for request!\n");

	while(1)
	{
		memset( &cliaddr, 0, sizeof(cliaddr) );
		cliaddrlen = sizeof(cliaddr);
		connfd = accept( sock, (struct sockaddr *)&cliaddr, (unsigned int *)&cliaddrlen );
		if(connfd < 0)
		{
			if(errno == EINTR)
				continue;
			else
				fprintf(stderr, "accept error\n");
		}
		
		if((child_pid = fork()) < 0)
		{
			fprintf(stderr, "创建新进程错误[%d]\n", child_pid);
			close(connfd);
			continue;
		}
		else if(child_pid == 0)
		{
			close(sock);
			/*do some work */
			str_echo(connfd);

			exit(0);
		}
		
		
		close(connfd);
		

	}/*end of while*/

	return 0;
}

/*
 * 初始化socket服务器
 *
 * @iPort	入参 端口号
 * @backlog	入参 监听连接个数
 *
 * 返回
 * 	n	正常
 *
 */

int init_socket_server(int iPort, int backlog)
{
	int	sock;
	char	sAddr[32];
	struct sockaddr_in seraddr;

	int	reuse = 1;

	sock = Socket(AF_INET, SOCK_STREAM, 0);

	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
	{
		fprintf(stderr, "setsockopt failed\n");
	}

	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_port = htons(iPort);

	Bind(sock, seraddr);

	Listen(sock, backlog);
	
	return (sock);
}


/*
 *回射函数，将客户端发过来的消息发送回去 
 *
 */
void str_echo(int sockfd)
{
	ssize_t	n;
	char	buf[MAXLINE];
	
again:
	while((n = read(sockfd, buf, MAXLINE)) > 0)
	{
		writen(sockfd, buf, n);
		fputs(buf, stdout);
	}
	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		fprintf(stderr, "str_echo:read error");

}
