/*
 * 功能：公共文件头
 *
 */

#ifndef _WYP_H_
#define _WYP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

/*socket错误编码*/
#define EOPENSOCK 	-61
#define EBIND		-62
#define ELISTEN		-63
#define ECONNECT	-64

/*****************错误级别************/
#define TRACE_ERROR	9, __FILE__, __LINE__  /***错误信息***/
#define TRACE_NORMAL	6, __FILE__, __LINE__  /***正常信息***/
#define TRACE_DEBUG	3, __FILE__, __LINE__ /***调试信息***/

#define MAXLINE 1024

/********** 函数声明 ************/
//int  Socket(int family, int type, int protocol);
//int  Bind(int sockfd, const struct sockaddr_in myaddr);
//int  Connect(int sockfd, const struct sockaddr_in servaddr);
//int  Listen(int sockfd, int backlog);
//ssize_t writen(int fd, const void *vptr, size_t n);



#endif
