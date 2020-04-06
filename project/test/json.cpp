/*
 *演示jsoncpp的基本使用：序列化和反序列化的过程
 */ 
#include<cstdio>
#include<iostream>
#include<string>
#include<jsoncpp/json/json.h>

int main()
{
  std::string name="小明";
  std::string sex="男";
  int age=26;

  Json::Value value;//json容器对象；可以直接通过key/val进行赋值
  value["姓名"]=name;
  value["性别"]=sex;
  value["年龄"]=age;
  value["成绩"].append(88);
  value["成绩"].append(77);
  value["成绩"].append(99);
  
  //FastWriter相较于StyledWriter序列化后可读性较弱
  Json::StyledWriter writer;//json序列化对象
  std::string json_str = writer.write(value);//StyledWriter.write将json::value对象进行序列化
  printf("json_str：[%s]\n",json_str.c_str());

  Json::FastWriter fwriter;//json序列化对象
  json_str = fwriter.write(value);
  printf("json_str：[%s]\n",json_str.c_str());

  Json::Value value2;
  Json::Reader reader;//json反序列化对象，可以实现将json数据传解析成为各个数据对象
  reader.parse(json_str,value2);//将数据串进行解析，得到数据对象放到json::value中
  std::cout<<"name:"<<value2["姓名"]<<std::endl;
  std::cout<<"sex:"<<value2["性别"]<<std::endl;
  std::cout<<"age:"<<value2["年龄"].asInt()<<std::endl;
  std::cout<<"score:"<<value2["成绩"]<<std::endl;
  for(auto it:value2["成绩"])
  {
    //因为成绩对象是一个数组，因此遍历获取其中内容
    std::cout<<"成绩："<<it<<std::endl;
  }
  return 0;
}
