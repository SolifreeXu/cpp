OBJS:=Connector/Connector.o test.o

# 把所有[.cpp]文件编译成[.o]文件
# -c 表示只编译源文件但不链接
# $< 表示所搜索到与第一个相匹配的文件，即第一个[.cpp]
# -o 指定输出文件名
# $@ 与[.cpp]文件相对应的[.o]文件
# -I 需要用到的头文件在此目录中寻找
Connector/%.o:Connector/%.cpp
	g++ -c $< -o $@ -IConnector/

%.o:%.cpp
	g++ -c $< -o $@ -I./

# -shared 此选项制定生成动态链接库
all:$(OBJS)
	@echo "Compiling"
	g++ -o mysql $(OBJS) -lmysqlclient
	@echo "End"

clean:
	rm -rf Connector/*.o *.o mysql

