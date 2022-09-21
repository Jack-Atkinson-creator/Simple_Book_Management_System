#include<Windows.h>
#include"Connection.h"
bool Connect::ConnectWithDB()
{
	error = SQLAllocEnv(&env);
	if (error != SQL_SUCCESS)
	{
		cout << "Environment allocation failed!" << endl;
		return false;
	}
	error = SQLAllocConnect(env, &conn);
	if (error != SQL_SUCCESS)
	{
		cout << "Connection allocation failed!" << endl;
		return false;
	}
	error= SQLConnect(conn, (SQLWCHAR*)L"Book_Management", SQL_NTS, (SQLWCHAR*)L"root", SQL_NTS, (SQLWCHAR*)L"root", SQL_NTS);
	if (error == SQL_SUCCESS || error == SQL_SUCCESS_WITH_INFO)
	{
		/*error = SQLAllocStmt(conn, &stmt);
		if (error != SQL_SUCCESS)
		{
			cout << "Statement handle allocation failed!" << endl;
			return false;
		}*/
		cout << "Welcome to our library!" << endl;
		return true;
	}
	else
	{
		cout << "Connection failed!" << endl;
		return false;
	}
}
void Connect::DisConnectWithDB()
{
	//SQLFreeStmt(stmt,SQL_DROP);
	SQLDisconnect(conn);
	SQLFreeConnect(conn);
	SQLFreeEnv(env);
}
