#include "Connector/Connector.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

const char host[] = "localhost";	// 主机地址
const unsigned port = 3306;	// 端口号，默认为3306
const char database[] = "custom";	// 数据库名
const char user[] = "root";	// 用户名
const char password[] = "eterfree";	// 密码

void visit(Connector::string data, void functor(const char*))
{
	using std::size_t;

	const char *begin = data.c_str();
	char *end;
	size_t row = static_cast<size_t>(std::strtoull(begin, &end, 10));
	size_t column = static_cast<size_t>(std::strtoull(end + 1, &end, 10));
	begin = end + 1;

	for (size_t i = 0; i < row; ++i)
	{
		for (size_t j = 0; j < column; ++j)
		{
			if (functor != nullptr)
				functor(begin);
			begin = std::strchr(begin, '\0') + 1;
		}
		functor(nullptr);
	}
}

void print(const char *string)
{
	if (string != nullptr)
		std::cout << string << '\t';
	else
		std::cout << std::endl;
}

// 程序入口函数
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "参数非法！\n";
		return 1;
	}

	Connector connector;
	connector.setHost(host);
	connector.setPort(port);
	connector.setDatabase(database);
	connector.setUser(user);
	connector.setPassword(password);
	if (!connector.connect())
		return 2;

	Connector::string *result = connector.execute(argv[1]);
	if (result)
	{
		visit(*result, print);
		delete result;
	}
	return 0;
}
