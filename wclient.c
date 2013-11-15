#include "wyp.h"

/****** 函数声明 ******/
//extern int  Socket(int family, int type, int protocol);
//extern int  Bind(int sockfd, const struct sockaddr_in myaddr);
//extern int  Connect(int sockfd, const struct sockaddr_in servaddr);


void str_cli(FILE *fp, int sockfd);


/*
 * 主函数
 *
 */
int main(void)
{
	int	i,sock;
	struct sockaddr_in servaddr;
	
	sock = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	if(inet_pton(AF_INET, "192.168.1.100", &servaddr.sin_addr) != 1)
		fprintf(stderr, "inet_pton error[%s][%d]", __FILE__, __LINE__);

	Connect(sock, servaddr);	
	
	str_cli(stdin, sock);

	exit(0);
}


/*
 * 从标准输入读取一行，写到服务器上，读回服务器对该行的回射
 *
 */

void str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];
	
	while(fgets(sendline, MAXLINE, fp) != NULL)
	{
		writen(sockfd, sendline, MAXLINE);
		
		readn(sockfd, recvline, MAXLINE);
		fputs(recvline, stdout);
	}
}
