#ifndef CUSTOMER_H
#define CUSTOMER_H
#include"Connection.h"
class customer
{
	string CurrID;
	string CurrPassword;//Current Password
public:
	void Register(Connect &x);
	void LogIn(Connect& x);
	void BorrowBook(Connect& x);
	void ReturnBook(Connect& x);
	void SearchBook(Connect& x);
	void PrintBookInfo(Connect &x,SQLWCHAR*t);
	void Run(Connect& x);
	void UpdateInfo(Connect& x);
	void ShowLibCard(Connect& x);
};
#endif