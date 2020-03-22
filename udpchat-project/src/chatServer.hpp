#pragma once 
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<string>
#include<queue>
#include"MsgPlool.hpp"
#define UDP_PORT 17777
#define THREAD_COUNT 2
//当前类实现
//1.接收用户数据
//2.发送给数据消息给客户端
//依赖UDP协议进行实现
class chatServer
{
  public:
    chatServer()
      :_udpSockfd(-1)
      ,_udpPort(UDP_PORT)
    {
      
    }
    ~chatServer()
    {

    }
    void InitServer()//初始化UDP服务
    {
      //1.创建套接字
      _udpSockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      if(_udpSockfd<0)
      {
        perror("socket error");
        exit(1);
      }
      //2.绑定地址信息
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(_udpPort);
      addr.sin_addr.s_addr=inet_addr("0.0.0.0");
      int ret = bind(_udpSockfd,(struct sockaddr*)&addr,sizeof(addr));
      if(ret<0)
      {
        perror("bind error");
        exit(2);
      }
    }
    void Start()//初始化生产和消费线程
    {
      pthread_t tid;
      for(int i=0;i<THREAD_COUNT;i++)
      {
        int ret = pthread_create(&tid,NULL,PrductMsStart,(void*)this);
        if(ret < 0)
        {
          perror("pthread error");
          exit(3);
        }
        ret = pthread_create(&tid,NULL,ConsumeMsStart,(void*)this);
        if(ret < 0)
        {
          perror("pthread error");
          exit(3);
        }    
      }
    }
    static void* PrductMsStart(void *arg)
    {
      pthread_detach(pthread_self());
      chatServer* cs = (chatServer*)arg;
      while(1)
      {
        //recvfrom()
        cs->RecvMsg();
      }
      return NULL;
    }
    static void* ConsumeMsStart(void* arg)
    {
      pthread_detach(pthread_self());
      while(1)
      {
         //sendto
        chatServer* cs = (chatServer*)arg;
        while(1)
        {
          //sendto
          cs->BroadcastMsg();
        }
      }
      return NULL;
    }
  private:
    int _udpSockfd;
    int _udpPort;
    //数据池对象
    MsgPool _msgPool;
    void RecvMsg()
    {
      char buf[1024]={0};
      struct sockaddr_in cli_addr;
      socklen_t cli_addlen=sizeof(struct sockaddr_in);
      int recvsize = recvfrom(_udpSockfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&cli_addr,&cli_addlen);
      if(recvsize<0)
      {
        perror("recvfrom error");
      }
      else
      {
          //正常逻辑
          
      }
    }
      void SendMsg(const std::string& msg,struct sockaddr_in& cli_addr,socklen_t len)
      {
        int sendsize = sendto(_udpSockfd,msg.c_str(),msg.size(),0,(struct sockaddr*)&cli_addr,len);
        if(sendsize<0)
        {
          perror("sendto error ");
        }
        else 
        {
          //成功
        }
      }
      void BroadcastMsg()
      {
        //获取要给哪一个用户进行发送
        //获取发送的内容
        //SendMsg();
      }
};
