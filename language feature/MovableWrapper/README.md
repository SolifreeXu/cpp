# 可移动封装器
用于突破语法限制，转换复制语义为移动语义。

## 功能
可移动封装器MovableWrapper具有智能指针std::unique_ptr的基本功能接口。  
不同的是，可移动封装器的复制语义也是移动操作。似此仅支持移动语义的类型转换为可移动封装器，便可以作为被强制要求复制构造和复制赋值的元素类型。

# 版本
当前版本：v1.0.0  
语言标准：C++11/C++14/C++17/C++20  
创建日期：2025年01月05日

# 作者
name：许聪  
mailbox：solifree@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/SolifreeXu
