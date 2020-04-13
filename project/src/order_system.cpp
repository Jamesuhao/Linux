#include "order_system.hpp"
#include"httplib.h"

_order_sys::DishTable *dish_table;
_order_sys::OrderTable *order_table;
void InsertDish(const httplib::Request &req,httplib::Response &rsp)
{
  //req.body中保存的正文---正文就是菜品信息的josn字符串
  Json::Value dish_val;

  Json::Reader reader;
  //reader.parse(json数据传，解析后的Json::Value对象);
  bool ret = reader.parse(req.body,dish_val);
  if(ret == false)
  {
    std::cout<<"InsertDish parse json data failed!"<<std::endl;
    rsp.status=400;
  }
  ret = dish_table->Insert(dish_val);//将菜品信息插入到数据库
  if(ret==false)
  {
    std::cout<<"InsertDish insert to db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  rsp.status=200;
  return;
}
void DeleteDish(const httplib::Request &req,httplib::Response &rsp)
{
  //path = "/dish/100";
  //匹配/dish/数字这种格式的字符串
  //matches[1]中保存所有匹配的指定字符串，matches[0]保存原始字符串
  int dish_id = std::stoi(req.matches[1]);
  
  bool ret = dish_table->Delete(dish_id);
  if(ret == false)
  {
    std::cout<<"DeleteDish delete dish from db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  rsp.status=200;
  return;
}
void UpdataDish(const httplib::Request &req,httplib::Response &rsp)
{
  //新的菜品信息保存在请求的正文中 --- 即就是req.body中
  Json::Value dish_val;

  Json::Reader reader;
  bool ret = reader.parse(req.body,dish_val);
  if(ret==false)
  {
    std::cout<<"UpdataDish parse json failed"<<std::endl;
    rsp.status=400;
    return;
  }
  ret = dish_table->Update(dish_val);
  if(ret==false)
  {
    std::cout<<"UpdataDish update to db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  rsp.status=200;
  return;
}
void SelectOneDish(const httplib::Request &req,httplib::Response &rsp)
{
  int dish_id=std::stoi(req.matches[1]);
  //从数据库获取指定id的菜品信息
  Json::Value dish_val;
  bool ret = dish_table->SelectOne(dish_id,&dish_val);
  if(ret==false)
  {
    std::cout<<"SelectOneDish select from failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  //获取成功，需要将dish_val中的数据对象，序列化称为json数据串，作为rsp.body 
  Json::FastWriter writer;
  std::string body = writer.write(dish_val);
  //正文就是菜品信息的json数据串
  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status=200;
  return;
}
void SelectAllDish(const httplib::Request &req,httplib::Response &rsp)
{
  Json::Value dishes_val;
  bool ret = dish_table->SelectAll(&dishes_val);
  if(ret == false)
  {
    std::cout<<"SelectAllDish select form db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  Json::FastWriter writer;
  std::string body = writer.write(dishes_val);
  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status=200;
  return;
}
void InsertOrder(const httplib::Request &req,httplib::Response &rsp)
{
  Json::Value order_val;
  Json::Reader reader;
  bool ret = reader.parse(req.body,order_val);
  if(ret == false)
  {
    std::cout<<"InsertOrder parse json failed!"<<std::endl;
    rsp.status=400;
    return;
  }
  ret = order_table->Insert(order_val);
  if(ret==false)
  {
    std::cout<<"InsertOrder insert data to db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  rsp.status=200;
  return;
}
void DeleteOrder(const httplib::Request &req,httplib::Response &rsp)
{
  int order_id=std::stoi(req.matches[1]);
  bool ret = order_table->Delete(order_id);
  if(ret == false)
  {
    std::cout<<"DeleteOrder delete order failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  rsp.status=200;
  return;
}
void UpdataOrder(const httplib::Request &req,httplib::Response &rsp)
{
  Json::Value order_val;
  Json::Reader reader;
  bool ret = reader.parse(req.body,order_val);
  if(ret == false)
  {
    std::cout<<"UpdataOrder parse json failed!"<<std::endl;
    rsp.status=400;
    return;
  }
  ret = order_table->Update(order_val);
  if(ret == false)
  {
    std::cout<<"UpdataOrder uptate data to db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  rsp.status=200;
  return;
}
void SeleteOneOrder(const httplib::Request &req,httplib::Response &rsp)
{
  int order_id=std::stoi(req.matches[1]);
  Json::Value order_val;
  bool ret = order_table->SelectOne(order_id,&order_val);
  if(ret==false)
  {
    std::cout<<"SeleteOneOrder get order info from db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  Json::FastWriter writer;
  std::string body = writer.write(order_val);
  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status=200;
  return;
}
void SelectAllOrder(const httplib::Request &req,httplib::Response &rsp)
{
  Json::Value orders_val;
  bool ret = order_table->SelectAll(&orders_val);
  if(ret == false)
  {
    std::cout<<"SeleteAllOrder get order info from db failed!"<<std::endl;
    rsp.status=500;
    return;
  }
  Json::FastWriter writer;
  std::string body=writer.write(orders_val);
  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status=200;
  return;
}

int main()
{
  MYSQL *mysql=_order_sys::MysqlInit();
  dish_table = new _order_sys::DishTable(mysql);
  order_table = new _order_sys::OrderTable(mysql);

  //服务器对象
  httplib::Server server;
  //菜品信息的管理
  //插入菜品请求      POST
  server.Post("/dish",InsertDish);
  //删除菜品请求      DELETE
  //R"(...)"：使用原始字符串，去除字符串中特殊字符的特殊含义
  server.Delete(R"(/dish/(\d+))",DeleteDish);
  //更新菜品请求      PUT
  server.Put("/dish",UpdataDish);
  //查询单个菜品请求  GET
  //R"(...)"：使用原始字符串，去除字符串中特殊字符的特殊含义
  server.Get(R"(/dish/(\d+))",SelectOneDish);
  //查询所有菜品请求  GET
  server.Get("/dish",SelectAllDish);
  
  //订单信息的管理
  //插入订单请求
  server.Post("/order",InsertOrder);
  //删除订单请求 
  //R"(...)"：使用原始字符串，去除字符串中特殊字符的特殊含义
  server.Delete(R"(/order/(\d+))",DeleteOrder);
  //更新订单请求
  server.Put("/order",UpdataOrder);
  //查询单个订单请求
  //R"(...)"：使用原始字符串，去除字符串中特殊字符的特殊含义
  server.Get(R"(/order/(\d+))",SeleteOneOrder);
  //查询所有订单请求
  server.Get("/order",SelectAllOrder);
  
  server.listen("0.0.0.0",9000);
  _order_sys::MysqlDestroy(mysql);
  return 0;
}
