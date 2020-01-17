//修改信号的处理方式2
//sigaction()接口
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

struct sigaction newact;
struct sigaction oldact;
void sigcb(int signum)
{
    printf("recv signum:%d\n",signum);
    sigaction(signum,&oldact,NULL);
}
int main()
{

    newact.sa_flags=0;
    newact.sa_handler=sigcb;
    sigemptyset(&newact.sa_mask);

    //sigaction修改信号的处理动作为newact,原来的动作使用oldact保存
    //int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
    sigaction(SIGINT,&newact,&oldact);
    while(1)
    {
        printf("hello\n");
        sleep(1);
    }
    return 0;
}
