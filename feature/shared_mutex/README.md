## 简介
参照C++17标准，采用C++11标准，封装共享互斥锁，兼容C++标准库。  
比如lock_guard或者unique_lock与shared_mutex组合，实现独占访问资源。

## 结构
文件|说明
-|-
[shared_mutex.hpp](shared_mutex.hpp)|定义共享互斥元类shared_mutex和共享互斥锁模板shared_lock。
[test.cpp](test.cpp)|测试代码。

## 功能
运用互斥元、互斥锁和条件变量模拟共享互斥元，保证共享状态的访问操作的原子性，并且实现线程调度，以控制线程访问顺序。

## 测试
创建10个线程，分别独占和共享访问资源，各线程分别打印正在访问资源的线程数量。

## 作者
name：许聪  
mailbox：solifree@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/SolifreeXu
