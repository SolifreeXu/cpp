共享互斥元件

## 简介
参考C++标准库，运用互斥元、互斥锁和条件变量，封装共享互斥元与共享互斥锁，可以补充C++11/C++14标准库。

## 功能
避免共享访问抢占独占访问，实现公平调度算法以确保线程序：
* 在共享访问之时，倘若发生独占访问，阻塞后续共享访问，等待独占访问结束。

## 说明
使用方法与标准库完全一致，例如：
1. 独占访问资源：组合使用lock_guard/unique_lock与shared_mutex/shared_timed_mutex。
2. 共享访问资源：组合使用shared_lock与shared_mutex/shared_timed_mutex。

## 版本
当前版本：v1.0.0
语言标准：C++11/C++14/C++17/C++20
创建日期：2025年02月03日

## 作者
name：许聪  
mailbox：solifree@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/SolifreeXu
