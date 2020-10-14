#include "Connector.h"

#include <sstream>
#include <iostream>

#pragma comment(lib, "libmysql.lib")

struct Connector::Structure
{
	using string = Connector::string;
	string host;
	Connector::uint_least16_t port;
	string database;
	string user;
	string password;
};

Connector::Connector()
	: table(std::make_unique<Structure>()) {}

Connector::~Connector()
{
	if (connector)
		disconnect();
}

void Connector::setHost(const string &host)
{
	table->host = host;
}

const Connector::string &Connector::getHost()
{
	return table->host;
}

void Connector::setPort(uint_least16_t port)
{
	table->port = port;
}

Connector::uint_least16_t Connector::getPort()
{
	return table->port;
}

void Connector::setDatabase(const string &database)
{
	table->database = database;
}

const Connector::string &Connector::getDatabase()
{
	return table->database;
}

void Connector::setUser(const string &user)
{
	table->user = user;
}

const Connector::string &Connector::getUser()
{
	return table->user;
}

void Connector::setPassword(const string &password)
{
	table->password = password;
}

const Connector::string &Connector::getPassword()
{
	return table->password;
}

// 初始连接
bool Connector::connect()
{
	using std::cerr;
	using std::endl;
	// 分配、初始化、返回MySQL连接实例对象地址
	if ((connector = mysql_init(nullptr)) == nullptr)
	{
		cerr << "初始连接实例失败！\n" << mysql_errno(connector) << ": " << mysql_error(connector) << endl;
		return false;
	}

	// 设置额外连接选项，这里MySQL字符集为utf8，若设置成功返回0，否则返回非0
	if (mysql_options(connector, MYSQL_SET_CHARSET_NAME, "utf8") != 0)
		cerr << "设置字符集失败！\n" << mysql_errno(connector) << ": " << mysql_error(connector) << endl;

	// 连接数据库，若连接成功，返回连接实例地址，否则返回NULL
	if (mysql_real_connect(connector,
		table->host.c_str(), 
		table->user.c_str(), 
		table->password.c_str(), 
		table->database.c_str(), 
		table->port, nullptr, 0) == nullptr)
	{
		cerr << "连接数据库失败！\n" << mysql_errno(connector) << ": " << mysql_error(connector) << endl;
		return false;
	}
	return true;
}

// 关闭连接
void Connector::disconnect()
{
	mysql_close(connector);
	connector = nullptr;
}

Connector::string *Connector::execute(const string &sql)
{
	using std::cerr;
	using std::endl;
	if (sql.empty())
	{
		cerr << "查询语句为空！" << endl;
		return nullptr;
	}

	std::lock_guard<std::mutex> locker(mutex);

	if (mysql_query(connector, sql.c_str()) != 0)
	{
		cerr << "查询出错！\n" << mysql_errno(connector) << ": " << mysql_error(connector) << endl;
		return nullptr;
	}

	MYSQL_RES *result = mysql_store_result(connector);	// 获取结果集
	if (result == nullptr)
	{
		cerr << "结果集为空！" << endl;
		return nullptr;
	}

	my_ulonglong rows = mysql_num_rows(result);	// 获取结果集行数
	if (rows <= 0)
	{
		mysql_free_result(result);	// 释放结果集
		return nullptr;
	}

	std::ostringstream buffer;
	unsigned fields = mysql_num_fields(result);
	buffer << rows + 1 << '\0' << fields << '\0';

	for (unsigned index = 0; index < fields; ++index)
	{
		MYSQL_FIELD *field = mysql_fetch_field(result);
		buffer << field->org_name << '\0';
	}

	MYSQL_ROW row = mysql_fetch_row(result);	// 指向结果集第一行数据
	while (row != nullptr)	// 如果行数据不为空
	{
		for (unsigned index = 0; index < fields; ++index)
		{
			if (row[index] != nullptr)
				buffer << row[index];
			buffer << '\0';
		}
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);
	return new string(buffer.str());
}
