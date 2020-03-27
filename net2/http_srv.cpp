//建一个http服务器，收到请求之后打印出来，然后组织一个响应hello world给客户端。
#include<iostream>
#include"tcpSocket.hpp"
#include<sstream>
#include<stdlib.h>
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
    std::string http_req;
    cli_sock.Recv(&http_req);
    std::cout<<"req:["<<http_req.c_str()<<"]"<<std::endl;
    //响应---首行(版本/状态码/描述)---头部(Content-Length)---空行----正文
    std::string body="<html><body><h1>Hello World</h1></body></html>";
    std::string blank = "\r\n";
    std::stringstream header;
    header<<"Content-Length: "<<body.size()<<"\r\n";
    header<<"Content-Ttpe: text/html\r\n";
    header<<"Location: http://www.baidu.com/\r\n";
    //std::string first_line="HTTP/1.1 200 OK\r\n";
    //std::string first_line="HTTP/1.1 302 Found\r\n";
    //std::string first_line="HTTP/1.1 302 Found\r\n";
    std::string first_line="HTTP/1.1 500 Internal Server Error\r\n";
    cli_sock.Send(first_line);
    cli_sock.Send(header.str());
    cli_sock.Send(blank);
    cli_sock.Send(body);
    cli_sock.Close();
  }
  listen_sock.Close();
  return 0;
}
