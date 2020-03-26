//通过封装的tcpSocket类实例化客户端对象实现tcp客户端
#include<iostream>
#include"tcpSocket.hpp"

int main(int argc,char* argv[])
{
  if(argc!=3)
  {
    std::cout<<"em:./tcp_cli 192.168.119.133 9000----服务端绑定地址"<<std::endl;
    return -1;
  }
  std::string ip=argv[1];
  uint16_t port=std::stoi(argv[2]);
  tcpSocket cli_sock;
  //创建套接字
  CHUNK_RET(cli_sock.Socket());
  //绑定地址信息(不推荐)
  //向服务端发起请求
  CHUNK_RET(cli_sock.Connect(ip,port));
  //循环收发数据
  int fd=cli_sock.GetFd();
  while(1)
  {
    cal_t tmp;//定义一个结构体变量，申请了一块空间
    //这个成员变量赋值的过程就是数据对象在内存中组织排布的过程
    tmp.num1=10;
    tmp.num2=20;
    tmp.op='+';
    send(fd,(void*)&tmp,sizeof(cal_t),0);//将这块空间的数据发送给服务端
    sleep(100);
  }
  cli_sock.Close();
  return 0;
}
