//使用封装的tcpSocket类实例化对象实现tcp服务端程序
#include<iostream>
#include"tcpSocket.hpp"

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
    //Accept是类成员函数，使用的私有成员_sockfd就是listen_sock的私有成员
    //cli_sock取地址传入，目的就是为了获取accept接口返回的通信套接字描述符
    //accept接口是一个阻塞函数，若没有新连接则阻塞等待
    bool ret = listen_sock.Accept(&cli_sock);//获取新套接字
    if(ret == false)
    {
      //获取新连接失败，可以重新继续获取下一个
      continue;
    }
    int fd=cli_sock.GetFd();
    cal_t tmp;//定义变量tmp申请了一块空间
    recv(fd,(void*)&tmp,sizeof(cal_t),0);
    int sum=tmp.num1+tmp.num2;
    std::cout<<tmp.num1<<" "<<tmp.op<<" "<<tmp.num2<<" "<<"="<<" "<<sum<<std::endl;
  }
  listen_sock.Close();
  return 0;
}
