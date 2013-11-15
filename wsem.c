/*
 * 信号量 操作函数
 *
 */
#include "wyp.h"

union semun{
	int		val;
	struct semid_ds *buf;
	ushort		*array;
};

#define SEM_MODE_PERM	((mode_t) 0600)

/*
 * 创建信号量
 *
 * @key		入参 IPC Key
 *
 * 返回：
 * 	n	信号量标识符
 * 	-1	创建信号量出错
 * 	-2	初始化信号量出错
 */
int SemCreate(key_t key)
{
	int	semid = 0;
	union	semun semarg;
	
	semid = semget(key, 1, IPC_CREAT | SEM_MODE_PERM);
	if(semid == -1)
		return -1;
	
	semarg.val = 1;
	if(semctl(semid, 0, SETVAL, semarg) == -1)
		return -2

	return semid;
}

/*
 * 查询信号量标识符
 *
 * @key		入参 IPC Key
 *
 * 返回：
 * 	n	信号量标识符
 * 	-1	出错
 */
int SemGet(key_t key)
{
	return semget(key, 0, SEM_MODE_PERM);
}

/*
 * 加锁信号量 P操作
 * 申请获得资源
 *
 * @key		入参 IPC Key
 *
 * 返回：
 * 	0	成功
 * 	-1	semop错误
 * 	-2	查询信号量标识符出错
 */
int SemLock(key_t key)
{
	int	semid = 0;
	struct sembuf popt = {0};
	
	popt.sem_num = 0;
	popt.sem_op = -1;
	popt.sem_flg = SEM_UNDO;

	semid = SemGet(key);
	if(semid == -1)
		return -2;
	
	if(semop(semid, &popt, 1) == -1)
		return -1;

	return 0;
}

/*
 * 解锁信号量 V操作
 * 申请释放资源
 *
 * @key		入参 IPC Key
 *
 * 返回：
 * 	0	成功
 * 	-1	semop错误
 * 	-2	查询信号量标识符出错
 */
int SemUnlock(key_t key)
{
	int	semid = 0;
	struct sembuf vopt = {0};
	
	vopt.sem_num = 0;
	vopt.sem_op = 1;
	vopt.sem_flg = SEM_UNDO | IPC_NOWAIT;

	semid = SemGet(key);
	if(semid == -1)
		return -2;
	
	if(semop(semid, &vopt, 1) == -1)
		return -1;

	return 0;
}

/*
 * 删除信号量
 *
 * @key		入参 IPC Key
 *
 * 返回：
 * 	0	成功
 * 	-1	删除出错
 * 	-2	查询信号量标识符出错
 */
int SemClose(key_t key)
{
	int	semid = 0;
	
	semid = SemGet(key);
	if(semid == -1)
		return -2;
	
	return (semctl(semid, 0, IPC_RMID, 0));
}
