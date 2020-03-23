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
      ,_msgPool(NULL)
    {  }
    ~chatServer()
    {
      if(_msgPool)
      {
        delete _msgPool;
        _msgPool=NULL;
      }
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
      //初始化数据池
      _msgPool=new MsgPool();
      if(!_msgPool)
      {
        perrror("Create MsgPool error");
        exit(3);
      }
    }
    void Start()//初始化生产和消费线程
    {
      pthread_t tid;
      for(int i=0;i<THREAD_COUNT;i++)
      {
        //创建生产者线程
        int ret = pthread_create(&tid,NULL,ProductMsStart,(void*)this);
        if(ret < 0)
        {
          perror("pthread_create product error");
          exit(4);
        }
        //创建消费者线程
        ret = pthread_create(&tid,NULL,ConsumeMsStart,(void*)this);
        if(ret < 0)
        {
          perror("pthread_create consumer error");
          exit(4);
        }    
      }
    }
    //生产者入口函数
    static void* ProductMsStart(void *arg)
    {
      //线程分离
      pthread_detach(pthread_self());
      chatServer* cs = (chatServer*)arg;
      while(1)
      {
        //recvfrom()
        cs->RecvMsg();
      }
      return NULL;
    }
    //消费者入口函数
    static void* ConsumeMsStart(void* arg)
    {
      //线程分离
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
    //接收数据
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
        std::string msg;
        msg.assign(buf,recvsize);
        _msgPool->PushMsgToPool(msg);
          
      }
    }
    //发送数据
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
    //获取发送信息以及进行发送
    void BroadcastMsg()
    {
        
      //获取要给哪一个用户进行发送
      //获取发送的内容
      //SendMsg();
      std::string msg;
      _msgPool->PopMsgFromPool(&msg);
    }
  private:
    int _udpSockfd;
    int _udpPort;
    //数据池对象
    MsgPool* _msgPool;
};
