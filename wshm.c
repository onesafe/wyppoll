/*
 * 共享内存 操作函数
 *
 */

#include "wyp.h"

#define SHM_FAILED	(void *)-1L
#define SHM_MODE_PERM	((mode_t) 0600)

/*
 * 创建共享内存
 *
 * @key		入参 IPC Key
 * @size	入参 要创建的共享内存大小
 *
 * 返回：
 * 	n	共享内存标识符
 * 	-1	出错
 *
 */
int ShmCreate(key_t key, int size)
{
	return shmget(key, size, IPC_CREAT | IPC_EXCL | SHM_MODE_PERM);
}

/*
 * 创建共享内存并挂接到当前进程
 *
 * @key		入参 IPC Key
 * @size	入参 要创建的共享内存大小
 * @addr	出参 存放共享内存首地址
 *
 * 返回:
 * 	n	共享内存标识符
 * 	-1	创建共享内存出错
 * 	-2	挂接共享内存出错
 */
int ShmCreateMount(key_t key, int size, void **addr)
{
	int	shmid = 0;
	
	shmid = shmget(key, size, IPC_CREAT | IPC_EXCL |SHM_MODE_PERM);
	if(shmid == -1)
		return -1;
	if((*addr = shmat(shmid, 0, 0)) == SHM_FAILED)
		return -2;
	
	return shmid;
}

/*
 * 查询共享内存标识符
 *
 * @key		入参 IPC Key
 *
 * 返回:
 * 	n 	共享内存标识符
 * 	-1	出错
 */
int ShmGet(key_t key)
{
	return shmget(key, 0, SHM_MODE_PERM);
}

/*
 * 挂接共享内存到当前进程，以只读方式
 *
 * @key		入参 IPC Key
 * @addr	出参 存放共享内存首地址
 *
 * 返回：
 * 	0 	成功
 * 	-1	挂接共享内存到当前进程错
 * 	-2	取共享内存标识符错
 */
int	ShmMountR(key_t key, void **addr)
{
	int	shmid = 0;
	
	if((shmid = ShmGet(key)) == -1)
		return -2;
	
	if((*addr = shmat(shmid, 0, SHM_RDONLY)) == SHM_FAILED)
		return -1;

	return 0;
}

/*
 * 挂接共享内存到当前进程，以读写方式
 *
 * @key		入参 IPC Key
 * @addr	出参 存放共享内存首地址
 *
 * 返回：
 * 	0	成功
 * 	-1	挂接共享内存到当前进程错
 * 	-2	取共享内存标识符错
 */
int ShmMountWR(key_t key, void **addr)
{
	int	shmid = 0;
	
	if((shmid = ShmGet(key)) == -1)
		return -2;
	
	if((*addr = shmat(shmid, 0, 0)) == SHM_FAILED)
		return -1;
	
	return 0;
}

/*
 * 从当前进程卸载共享内存
 *
 * @addr	入参 共享内存首地址
 *
 * 返回：
 * 	0	成功
 * 	-1	出错
 */
int ShmUnMount(void *addr)
{
	return shmdt(addr);
}

/*
 * 删除共享内存
 *
 * @key		入参 IPC Key
 *
 * 返回：
 * 	0	成功
 * 	-1	删除出错
 * 	-2	查询共享内存标识符出错
 */
int ShmClose(key_t key)
{
	int	shmid = 0;
	
	shmid = ShmGet(key);
	if(shmid == -1)
		return -2;
	
	return shmctl(shmid, IPC_RMID, 0);
}
