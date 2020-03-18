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
  while(1)
  {
    std::cout<<"client say：";
    fflush(stdout);
    std::string buf;
    std::cin>>buf;
    
    //因为客户端不存在多种套接字的文件，因此一旦当前套接字出错直接退出就行
    //进程退出就会释放资源，关闭套接字
    CHUNK_RET(cli_sock.Send(buf));
    buf.clear();
    CHUNK_RET(cli_sock.Recv(&buf));
    std::cout<<"server say："<<buf.c_str()<<std::endl;
  }
  cli_sock.Close();
  return 0;
}
