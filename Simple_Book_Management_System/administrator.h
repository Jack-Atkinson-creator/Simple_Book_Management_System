#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H
#include"Connection.h"
 class administrator
 {
	 string CurrID;
	 string CurrPass;
 public:
	 void Run(Connect& x);
	 void LogIn(Connect& x);
	 void ShowUserInf(Connect& x);
	 void AddUser(Connect& x);
	 void DropUser(Connect& x);
	 void ShowAdminInf(Connect& x);
	 void AddAdmin(Connect& x);
	 void DropAdmin(Connect& x);
	 void ShowBookInf(Connect& x);
	 void Purchase(Connect& x);
	 void ClearInventory(Connect& x);
	 //Clear the redundant borrow information.
	 void ClearBorrowInf(Connect& x);
	 void ShowBorrowInf(Connect& x);
 };
#endif
