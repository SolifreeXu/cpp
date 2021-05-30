# 简介
参照C++标准，封装位集合，不仅能够节省内存，还支持动态扩容。

# 功能
位集合自定义元素类型，提供位、集合两种位运算，暂不支持移位运算。
> 集合位运算：仅限于相同容量的集合。

# 用例
1. 位集合如何移除另一位集合含有的元素？
二者依次进行或运算和异或运算。

# 文件
* [BitSet.hpp](BitSet.hpp)：定义位集合类模板BitSet。
* [test.cpp](test.cpp)：测试代码。

# 作者
name：许聪  
mailbox：2592419242@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee：https://gitee.com/solifree  
github：https://github.com/xucongandxuchong
