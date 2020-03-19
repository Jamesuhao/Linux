//使用封装的tcpSocket类实例化对象实现tcp服务端程序
#include<iostream>
#include"tcpSocket.hpp"
#include<signal.h>
#include<sys/wait.h>
void sigcb(int signo)
{
  //当子进程退出的时候，就会向父进程发送SIGCHLD信号，回调这个函数
  //waitpid返回值>0表示处理了一个退出的子进程
  //waitpid<=0表示没有退出的子进程了
  while(waitpid(-1,0,WNOHANG)>0);//一次回调循环将退出的子进程处理点
}
int main(int argc,char *argv[])
{
  if(argc!=3)
  {
    std::cout<<"em:./tcp_srv 192.168.199.133  9000"<<std::endl;
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = std::stoi(argv[2]);//stoi：将字符串转换为数字
  signal(SIGCHLD,sigcb);
  tcpSocket listen_sock;//监听套接字对象
  CHUNK_RET(listen_sock.Socket());//创建套接字
  CHUNK_RET(listen_sock.Bind(ip,port));//绑定地址信息
  CHUNK_RET(listen_sock.Listen());//开始监听
  while(1)
  {
    tcpSocket cli_sock;//通信套接字
    std::string cli_ip;
    uint16_t cli_port;
    //Accept是类成员函数，使用的私有成员_sockfd就是listen_sock的私有成员
    //cli_sock取地址传入，目的就是为了获取accept接口返回的通信套接字描述符
    //accept接口是一个阻塞函数，若没有新连接则阻塞等待
    bool ret = listen_sock.Accept(&cli_sock,&cli_ip,&cli_port);//获取新套接字
    if(ret == false)
    {
      //获取新连接失败，可以重新继续获取下一个
      continue;
    }
    std::cout<<"new connect：["<<cli_ip.c_str()<<"："<<cli_port<<"]"<<std::endl;
    pid_t pid=fork();//子进程复制父进程---数据独有，代码共享
    if(pid == 0)
    {
      //让子进程与客户端通信
      while(1)
      {
        std::string buf;
        if(cli_sock.Recv(&buf)==false)
        {
          cli_sock.Close();
          exit(0);
        }
        std::cout<<"client：["<<&cli_ip[0]<<":"<<cli_port<<"] say："<<&buf[0]<<std::endl;
        std::cout<<"server say：";
        fflush(stdout);
        buf.clear();
        std::cin>>buf;
        if(cli_sock.Send(buf)==false)
        {
          cli_sock.Close();
          exit(0);
        }
      }
      cli_sock.Close();
      exit(0);
    }
    //父子进程数据独有，都会具有cli_sock,但是父进程并不通信
    cli_sock.Close();//这个关闭对子进程没有影响，因为数据各自有一份
    
  }
  listen_sock.Close();
  return 0;
}
