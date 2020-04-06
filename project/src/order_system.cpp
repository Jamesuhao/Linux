#include "order_system.hpp"
int main()
{
  MYSQL* mysql = _order_sys::MysqlInit();
  if(mysql==NULL)
  {
    return -1;
  }
  _order_sys::DishTable dish_tb(mysql);
  _order_sys::OrderTable order_tb(mysql);
 
  /*
   * 插入菜品信息
  Json::Value dish;
  dish["name"]="红烧肉";
  dish["price"]=3600;
  dish_tb.Insert(dish);
  */

  /*
   * 查询单个菜品信息
  Json::Value dish1;
  Json::StyledWriter writer;
  dish_tb.SelectOne(4,&dish1);
  std::cout<<"dish:"<<writer.write(dish1)<<std::endl;
  */
 
  /*
   * 查询所有菜品信息
  Json::Value dish2;
  Json::StyledWriter writer2;
  dish_tb.SelectAll(&dish2);
  std::cout<<"dish:"<<writer2.write(dish2)<<std::endl;
  */
  
  /*
   * 更新菜品信息
  Json::Value dish3;
  dish3["id"]=6;
  dish3["name"]="唐僧肉";
  dish3["price"]=6800;
  dish_tb.Update(dish3);
  */

  /*
   * 删除菜品信息
  dish_tb.Delete(8);
  */
  
  //订单数据管理测试
  Json::Value order;
  Json::StyledWriter writer;

  /*
   * 订单插入测试
  order["table_id"]=5;
  order["dishes"].append(6);
  order["dishes"].append(7);
  order["status"]=0;
  order_tb.Insert(order);
   */

  
  /*
   * 查询单个订单信息
  Json::Value order1;
  order_tb.SelectOne(1,&order1);
  std::cout<<"order:"<<writer.write(order1)<<std::endl;
  */

  /*查询所有订单信息
  Json::Value order2;
  order_tb.SelectAll(&order2);
  std::cout<<"order:"<<writer.write(order2)<<std::endl;
  */
  
  /*更新订单信息
  order["id"]=1;
  order["table_id"]=3;
  order["dishes"].append(6);
  order["status"]=0;
  order_tb.Update(order);
  */

  /*
   * 删除订单信息
  order_tb.Delete(1);
  */ 
  _order_sys::MysqlDestroy(mysql);
  return 0;
}
