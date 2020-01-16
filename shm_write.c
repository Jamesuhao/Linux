//向共享内存写入数据
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/shm.h>
#define IPC_KEY 0X12345678
#define PROJ_ID 0X12345678
int main()
{
    //创建共享内存
    /*生成key值的函数接口
    key_t key - ftok("./",PROJ_ID);
    int shmid = shmget(key,32,IPC_CREAT|0664);
    */
    int shmid = shmget(IPC_KEY,32,IPC_CREAT|0664);
    if(shmid < 0)
    {
        perror("shmget fail");
        return -1;
    }
    //映射到虚拟地址
    void* shmstart = shmat(shmid,NULL,0);
    if(shmstart == (void*)-1)
    {
        perror("shmat error");
        return -1;
    }
    //像共享内存中写入数据
    int i=0;
    while(1)
    {
        sprintf((char*)shmstart,"%s-%d\n","share memory",i++);
        sleep(1);
    }
    //解除映射关系
    shmdt(shmstart);
    //释放共享内存
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}
