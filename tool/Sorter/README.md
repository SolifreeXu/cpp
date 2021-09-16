## 简介
灵感突如其来，以无序映射和有序集合封装排序者类模板。

## 结构
文件|说明
-|-
[Sorter.hpp](Sorter.hpp)|定义排序者类模板Sorter。
[test.cpp](test.cpp)|测试代码。

## 功能
采用共享互斥元，以支持多线程调用成员函数，从而提高并发性能。提供查询、更新、移除、备份记录等方法。

公有函数|说明
-|-
size|获取记录数量。
find|获取指定ID的原始记录。
get|获取指定起始位置的指定数量的记录列表。
update|新增或者更新记录。
remove|移除单条记录，返回移除数量。
clear|清空排序者数据。
backup|备份排序者。

## 测试
1. 定义记录结构体，并且重载小于运算符。
2. 创建排序者对象，插入100条记录之后备份排序者。
3. 更新两条记录，打印原始排序者前10条记录，以及备份排序者指定ID的记录。

## 作者
name：许聪  
mailbox：2592419242@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/xucongandxuchong
