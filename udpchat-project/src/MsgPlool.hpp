#pragma once
#include<iostream>
#include<pthread.h>
#include<queue>
#include<string>
#define MSG_POOL_SIZE 1024
class MsgPool
{
  public:
    MsgPool()
      :_Capacity(MSG_POOL_SIZE) 
    {
      pthread_mutex_init(&_MsgQueLock,NULL);
      pthread_cond_init(&_SynConmQue,NULL);
      pthread_cond_init(&_SynProQue,NULL);
    }
    ~MsgPool()
    {
      pthread_mutex_destroy(&_MsgQueLock);
      pthread_cond_destroy(&_SynConmQue);
      pthread_cond_destroy(&_SynProQue);
    }
    void PushMsgToPool(std::string& msg)
    {
      pthread_mutex_lock(&_MsgQueLock);
      while(Isfull())
      {
        pthread_cond_wait(&_SynProQue,&_MsgQueLock);
      }
      _MsgQue.push(msg);
      pthread_mutex_unlock(&_MsgQueLock);
      pthread_cond_signal(&_SynConmQue);
    }
    void PopMsgFromPool(std::string* msg)
    {
      pthread_mutex_lock(&_MsgQueLock);
      while(_MsgQue.empty())
      {
        pthread_cond_wait(&_SynConmQue,&_MsgQueLock);
      }
      *msg=_MsgQue.front();
      _MsgQue.pop();
      pthread_mutex_unlock(&_MsgQueLock);
      pthread_cond_signal(&_SynProQue);
    }
  private:
    bool Isfull()
    {
      if(_MsgQue.size()==_Capacity)
      {
        return true;
      }
      return false;
    }
  private:
    std::queue<std::string>_MsgQue;
    size_t _Capacity;//约束队列大小，防止异常情况下，队列无限扩容，导致内存占用过大或者被操作系统强杀
    pthread_mutex_t _MsgQueLock;//互斥锁
    pthread_cond_t _SynConmQue;//消费者条件变量
    pthread_cond_t _SynProQue;//生产者条件变量
};
