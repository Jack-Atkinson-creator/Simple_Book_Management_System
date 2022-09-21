#ifndef CONNECTION_H
#define CONNECTION_H
#include<Windows.h>
#include<iostream>
#include<sql.h>
#include<sqlext.h>
#include<string>
#include<time.h>
#define MAX_LEN 2048
#define Column_Book 6
#define USER_INF 6
using namespace std;
class Connect
{
	RETCODE error;//Test status
	HENV env;//environment
	SQLHDBC conn;//connection
	SQLHSTMT  stmt;  //Statement Handle

public: 
	bool ConnectWithDB();
	void DisConnectWithDB();
	friend class customer;
	friend class administrator;
};

#endif