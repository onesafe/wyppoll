/*
 * 功能：打印日志
 *
 */

#include "wyp.h"

static int	gnLogType = 1;	/* 0:默认为标准输出；1:自定义 */

static char	gsLogFile[LOG_FILE_LEN + 1];  /* 日志全路径及文件名 */

/********** 外部函数声明  **********/
extern char *BuildFile(const char *file);
extern char *TimeNow(char *buf);
extern char *DateNow(char *buf);
extern char *BufToStr(const char *buf, unsigned int len);


/*
 * 打印日志
 *
 * @level	入参
 * @src_file	入参
 * @src_line	入参
 * @fmt		入参
 * @...		入参
 *
 * 返回：
 * 	0	成功
 * 	-1	open错
 * 	-2	配置错
 */
int Log(int level, char *src_file, int src_line, char *fmt, ...)
{
	int	fd;
	char	msg[1024 + 128] = {0};
	char	buf[1024 + 128] = {0};

	char	*pdate = DateNow(NULL);
	char	*ptime = TimeNow(NULL);
	va_list	args;

	switch(gnLogType)
	{
		case 0:
			fd = STDOUT_FILENO;
			break;
		case 1:
			memset(gsLogFile, 0, sizeof(gsLogFile));
			strcpy(gsLogFile, BuildFile("./log/logfile"));
			fd = open(gsLogFile, O_RDWR | O_APPEND | O_CREAT, 0644);
			if(fd == -1)
				return -1;
			break;
		default:
			return -2;
	}

	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	va_end(args);

	sprintf(buf + strlen(buf), "[PID=%5d]", getpid());

	sprintf(buf + strlen(buf), "[%4s/", BufToStr(pdate, 4));
	sprintf(buf + strlen(buf), "%2s", BufToStr(pdate + 4, 2));
	sprintf(buf + strlen(buf), "/%2s]", BufToStr(pdate + 4, 2));
	
	sprintf(buf + strlen(buf), "[%2s", BufToStr(ptime, 2));
	sprintf(buf + strlen(buf), ":%2s", BufToStr(ptime + 2, 2));
	sprintf(buf + strlen(buf), ":%2s]", BufToStr(ptime + 4, 2));

//	sprintf(buf + strlen(buf), "[%15s: %04d]", src_file, src_line);

	if(strlen(buf) == 0)
		sprintf(buf, "%s\n", msg);
	else
		sprintf(buf + strlen(buf), " %s\n", msg);
	
	write(fd, buf, strlen(buf));
	
	if(fd != STDOUT_FILENO)
		close(fd);
	
	return 0;
}
