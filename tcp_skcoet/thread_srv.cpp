//使用封装的tcpSocket类实例化对象实现tcp服务端程序
#include<iostream>
#include<pthread.h>
#include"tcpSocket.hpp"


void* thr_start(void *arg)
{
  long fd=(long)arg;
  tcpSocket cli_sock;
  cli_sock.SetFd(fd);
  while(1)
  {
    std::string buf;
    if(cli_sock.Recv(&buf)==false)
    {
      cli_sock.Close();
      pthread_exit(NULL);
    }
    std::cout<<"client：say："<<&buf[0]<<std::endl;
    std::cout<<"server say：";
    fflush(stdout);
    buf.clear();
    std::cin>>buf;
    if(cli_sock.Send(buf)==false)
    {
      cli_sock.Close();
      pthread_exit(NULL);
    }
  }
  //当循环退出时则关闭套接字
  cli_sock.Close();
  return NULL;
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
    //通过新获取的通信套接字与客户端进行通信
    pthread_t tid;
    //将通信套接字当作参数传递给线程，让线程与数据进行通信
    //cli_sock是一个局部变量，循环结束，这个资源就会被被释放掉
    pthread_create(&tid,NULL,thr_start,(void*)cli_sock.GetFd());
    pthread_detach(tid);//不关心线程返回值，分离线程，退出后自动释放资源
    //主线程不能关闭cli_sock套接字，因为多线程是共用描述符的
  }
  listen_sock.Close();
  return 0;
}
