## 简介
参照C++14、C++17标准，采用C++11标准，运用互斥元、互斥锁和条件变量，封装共享互斥元与共享互斥锁，可以补充C++11标准库。

## 功能
为确保线程顺序，避免共享访问一直抢占，实现公平调度算法：
* 在共享访问之时，倘若发生独占访问，阻塞后续共享访问，等待独占访问结束。

## 说明
使用方法与标准库完全一致，例如：
1. 独占访问资源：组合使用lock_guard/unique_lock与shared_mutex/shared_timed_mutex。
2. 共享访问资源：组合使用shared_lock与shared_mutex/shared_timed_mutex。

## 作者
name：许聪  
mailbox：solifree@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/SolifreeXu
