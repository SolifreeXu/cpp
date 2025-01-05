#pragma once

#include <mysql/mysql.h>

#include <memory>
#include <cstdint>
#include <string>
#include <mutex>

class Connector
{
	struct Structure;

	MYSQL *connector = nullptr;	// MySQL连接实例指针
	std::unique_ptr<Structure> table;
	std::mutex mutex;
public:
	using string = std::string;
	using uint_least16_t = std::uint_least16_t;
	Connector();
	~Connector();
	void setHost(const string &host);
	const string &getHost();
	void setPort(uint_least16_t port);
	uint_least16_t getPort();
	void setDatabase(const string &database);
	const string &getDatabase();
	void setUser(const string &user);
	const string &getUser();
	void setPassword(const string &password);
	const string &getPassword();
	bool connect();
	void disconnect();
	string *execute(const string &sql);
};
