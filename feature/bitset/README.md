# 简介
参照C++标准库，封装新位集合，具有更高的灵活性与实用性。

# 功能
1. 自定义无符号整型元素，节省内存而不受字节序影响。  
2. 支持动态扩大容量，提供位与集合两种位运算。

# 用例
1. 位集合如何移除另一位集合含有的元素？  
二者依次进行或运算和异或运算。  
2. 位集合如何实现旋转？  
分别左移与右移，得到两个中间位集合，二者进行或运算。

# 版本
* 语言标准：C++20
* 当前版本：v1.0.1
## 变化
**v1.0.1**  
1. 新增范围设置、重置、翻转、复制，以及移位运算。

# 文件
* [BitSet.hpp](src/BitSet.hpp)：定义位集合类模板BitSet。
* [test.cpp](test/test.cpp)：测试代码。

# 作者
name：许聪  
mailbox：solifree@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/xucongandxuchong
