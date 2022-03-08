# 简介
参照C++标准，封装位集合，自定义无符号整型元素，节省内存而不受字节序影响。

# 功能
位集合支持动态扩大容量，提供位与集合两种位运算。

# 用例
1. 位集合如何移除另一位集合含有的元素？  
二者依次进行或运算和异或运算。  
2. 位集合如何实现旋转？  
分别左移与右移，得到两个中间位集合，二者进行或运算。

# 文件
* [BitSet.hpp](BitSet.hpp)：定义位集合类模板BitSet。
* [test.cpp](test.cpp)：测试代码。

# 作者
name：许聪  
mailbox：solifree@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/xucongandxuchong
