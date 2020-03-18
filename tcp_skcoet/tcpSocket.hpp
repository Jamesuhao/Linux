//封装实现一个tcpSocket类，向外提供简单接口；
//使外部通过实例化一个tcpSocket对象就能完成tcp通信程序的建立
#include<iostream>
#include<stdlib.h>
#include<cstdio>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string>
#include<arpa/inet.h>
#define BACKLOG 10
#define CHUNK_RET(q) if((q)==false){return -1;}
class tcpSocket
{
  public:
    tcpSocket()
      :_sockfd(-1)
    { }
    //1.创建套接字
    bool Socket()
    {
      //socket(地址域，套接字类型，协议类型)
      _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(_sockfd<0)
      {
        perror("socket error");
        return false;
      }
      return true;
    } 
    //2.绑定地址信息
    void Addr(struct sockaddr_in* addr,const std::string &ip,const uint16_t port)
    {
      addr->sin_family = AF_INET;
      addr->sin_port = htons(port);
      inet_pton(AF_INET,ip.c_str(),&(addr->sin_addr.s_addr));
    }
    bool Bind(const std::string &ip,const uint16_t port)
    {
      //1.定义ipv4地址结构
      struct sockaddr_in addr;
      Addr(&addr,ip,port);
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd,(struct sockaddr*)&addr,len);
      if(ret<0)
      {
        perror("bind error");
        return false;
      }
      return true;
    }
    //3.服务端开始监听
    bool Listen(int backlog = BACKLOG)
    {
      //listen(描述符，同一时间的并发连接数);
      int ret = listen(_sockfd,backlog);
      if(ret < 0)
      {
        perror("listen error");
        return false;
      }
      return true;
    }
    //4.客户端发起连接请求
    bool Connect(const std::string &ip,const uint16_t port)
    {
      //1.定义ipv4地址结构，赋予服务端地址信息
      struct sockaddr_in addr;
      Addr(&addr,ip,port);
      //2.向服务端发起请求
      //3.connect(客户端描述符，服务端地址信息，地址长度)
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = connect(_sockfd,(struct sockaddr*)&addr,len);
      if(ret < 0)
      {
        perror("connect error");
        return false;
      }
      return true;
    }
    //5.服务端获取新建链接
    bool Accept(tcpSocket* sock,std::string *ip=nullptr,uint16_t *port=nullptr)
    {
      //accept(监听套接字，对端地址信息，地址信息长度)，返回新的描述符
      struct sockaddr_in addr;
      socklen_t len = sizeof(struct sockaddr_in);
      //获取新的套接字，以及这个套接字对应的对端地址信息
      int clisockfd = accept(_sockfd,(struct sockaddr*)&addr,&len);
      if(clisockfd < 0)
      {
        perror("accept error");
        return false;
      }
      //用户传入了一个tcpSocket对象的指针，为这个对象描述符进行赋值---赋值为新建套接字的描述符
      //后续与客户端的通信通过这个对象可以完成
      sock->_sockfd = clisockfd;
      if(ip!=nullptr)
      {
        *ip=inet_ntoa(addr.sin_addr);//网络字节序->字符串
      }
      if(port!=nullptr)
      {
        *port=ntohs(addr.sin_port);
      }
      return true;
    }
    //6.发送数据
    bool Send(const std::string &data)
    {
      //send(描述符，数据，数据长度，选项参数)
      int ret = send(_sockfd,data.c_str(),data.size(),0);
      if(ret < 0)
      {
        perror("send error");
        return false;
      }
      return true;
    }
    //7.接收数据
    bool Recv(std::string *buf)
    {
      //recv(描述符，缓冲区，数据长度，选项参数)
      char tmp[4096]= {0};
      int ret = recv(_sockfd,tmp,4096,0);
      if(ret < 0)
      {
        perror("recv error");
        return false;
      }
      else if(ret == 0)
      {
        std::cout<<"connect break"<<std::endl;
        return false;
      }
      buf->assign(tmp,ret);//从tmp中拷贝ret大小的数据到buf中
      return true;
    }
    //8.关闭套接字
    bool Close()
    {
      close(_sockfd);
      _sockfd=-1;
      return true;
    }
  private:
    int _sockfd;
};
