#define _CRT_SECURE_NO_WARNINGS
#include"administrator.h"
using namespace std;
void administrator::Run(Connect& x)
{
	while (1)
	{
		string input;
		cout << "1.LogIn;0.Exit" << endl;
		cin >> input;
		if (input == "1")
		{
			LogIn(x);
		}
		else if (input == "0")
		{
			break;//exit
		}
		else
		{
			cout << "Invalid input.Please try again." << endl;
		}
	}
}
void administrator::LogIn(Connect& x)
{
	cout << "Please input your ID" << endl;
	string id, pass;
	SQLWCHAR tmp[MAX_LEN];//Transition, type conversion.
	cin >> id;
	cout << "Please input your password" << endl;
	cin >> pass;
	string s = "select admin_password from administrator where admin_id='";
	s += (id + "';");
	swprintf(tmp, MAX_LEN, L"%hs", s.c_str());

	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
	if (x.error == SQL_SUCCESS)
		//Find the password.
	{
		char RightPass[21];//The max length of password is 20.
		int lengths;
		SQLBindCol(x.stmt, 1, SQL_C_CHAR, RightPass, 21, (SQLINTEGER*)&lengths);
		if (SQLFetch(x.stmt) != SQL_SUCCESS)
			//ID not exist
		{
			cout << "The ID does not exist." << endl;
			SQLFreeStmt(x.stmt, SQL_DROP);
			return;
		}
		else
		{
			string right(RightPass);
			SQLFreeStmt(x.stmt, SQL_DROP);
			if (pass == right)//Password correct!
			{
				cout << "Log In success!" << endl;
				CurrID = id;
				CurrPass = pass;//Record current ID and passwords.
				while (1)
				{
					string input, sinput;
					cout << "Please input your tasks" << endl;
					cout << "1.User management;\n2.Book management;\n3.Administrator structure adjustment;\n4.Show borrow information table;\n5.Clear the redundant borrow information;\n0.Exit." << endl;
					cin >> input;
					if (input == "1")
					{
						cout << "Please enter what you want to do;" << endl;
						cout << "1.Show user information.\n2.Add a user.\n3.Delete a user.\n0.Exit User Management" << endl;
						cin >> sinput;
						while (sinput != "0")
						{
							if (sinput == "1")
							{
								ShowUserInf(x);
							}
							else if (sinput == "2")
							{
								AddUser(x);
							}
							else if (sinput == "3")
							{
								DropUser(x);
							}
							else
							{
								cout << "Invalid input.Please try again." << endl;
							}
							cout << "Please enter what you want to do;" << endl;
							cout << "1.Show user information.\n2.Add a user.\n3.Delete a user.\n0.Exit User Management" << endl;
							cin >> sinput;
						}
					}
					else if (input == "2")
					{
						cout << "Please enter what you want to do;" << endl;
						cout << "1.Show book information.\n2.Purchase books.\n3.Clear superfluous books.\n0.Exit Book Management" << endl;
						cin >> sinput;
						while (sinput != "0")
						{
							if (sinput == "1")
							{
								ShowBookInf(x);
							}
							else if (sinput == "2")
							{
								Purchase(x);
							}
							else if (sinput == "3")
							{
								ClearInventory(x);
							}
							else
							{
								cout << "Invalid input.Please try again." << endl;
							}
							cout << "Please enter what you want to do;" << endl;
							cout << "1.Show book information.\n2.Purchase books.\n3.Clear superfluous books.\n0.Exit Book Management" << endl;
							cin >> sinput;
						}
					}
					else if (input == "3")
					{
						cout << "Please enter what you want to do;" << endl;
						cout << "1.Show administrator information.\n2.Add an administrator.\n3.Delete an administrator.\n0.Exit Administrator structure adjustment" << endl;
						cin >> sinput;
						while (sinput != "0")
						{
							if (sinput == "1")
							{
								ShowAdminInf(x);
							}
							else if (sinput == "2")
							{
								AddAdmin(x);
							}
							else if (sinput == "3")
							{
								DropAdmin(x);
							}
							else
							{
								cout << "Invalid input.Please try again." << endl;
							}
							cout << "Please enter what you want to do;" << endl;
							cout << "1.Show administrator information.\n2.Add an administrator.\n3.Delete an administrator.\n0.Exit Administrator structure adjustment" << endl;
							cin >> sinput;
						}
					}
					else if (input == "4")
					{
						ShowBorrowInf(x);
					}
					else if (input == "5")
					{
						ClearBorrowInf(x);
					}
					else if (input == "0")
					{
						break;//exit
					}
					else
					{
						cout << "Invalid input.Please try again." << endl;
					}
				}

			}
			else
			{
				cout << "The password is invalid!" << endl;
				return;
			}
		}
	}
	else
	{
		cout << "The search has an unexpected error!" << endl;
		return;
	}
}
void administrator::ShowUserInf(Connect& x)
{
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	string s = "select*from customer natural left join customer_phone;";
	SQLWCHAR t[MAX_LEN];
	swprintf(t, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		short i, k = 1;
		char Temp[USER_INF][51];
		char Res[MAX_LEN][USER_INF][51];//store the result.
		SQLINTEGER Maxlength[USER_INF];//max-width with corresponding column.The
		SQLINTEGER TL[USER_INF];
		//Intialize
		Maxlength[0] = strlen("customer_id"); strcpy(Res[0][0], "customer_id");
		Maxlength[1] = strlen("customer_name"); strcpy(Res[0][1], "customer_name");
		Maxlength[2] = strlen("customer_password"); strcpy(Res[0][2], "customer_password");
		Maxlength[3] = strlen("customer_sex"); strcpy(Res[0][3], "customer_sex");
		Maxlength[4] = strlen("phone_number"); strcpy(Res[0][4], "phone_number");
		Maxlength[5] = strlen("location"); strcpy(Res[0][5], "location");
		//bind
		for (i = 1; i <= 6; i++)
		{
			SQLBindCol(x.stmt, i, SQL_C_CHAR, Temp[i - 1], MAX_LEN, &TL[i - 1]);
		}

		while (SQLFetch(x.stmt) == SQL_SUCCESS)
		{
			
			for (i = 0; i < 6; i++)
			{
				if (TL[i] == -1)//NULL
				{
					strcpy(Temp[i], "NULL");
				}
				strcpy(Res[k][i], Temp[i]);
				if (strlen(Temp[i]) > Maxlength[i])
				{
					Maxlength[i] = strlen(Temp[i]);//Update max.
				}
			}
			k++;
		}
		//First Line
		printf("+");
		for (i = 0; i < 6; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Second line
		printf("|");
		for (i = 0; i < 6; i++)
		{
			printf("%s", Res[0][i]); //Attribute.
			int Remain = Maxlength[i] - strlen(Res[0][i]);
			for (int j = 0; j < Remain; j++)
			{
				printf(" ");
			}
			printf("|");
		}
		printf("\n");
		//3-rd Line
		printf("+");
		for (i = 0; i < 6; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Data
		for (int j = 1; j < k; j++)
		{
			printf("|");
			for (i = 0; i < 6; i++)
			{
				printf("%s", Res[j][i]);
				int Remain = Maxlength[i] - strlen(Res[j][i]);
				for (int l = 0; l < Remain; l++)
				{
					printf(" ");
				}
				printf("|");
			}
			printf("\n");
		}
		//Final line
		printf("+");
		for (i = 0; i < Column_Book; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
	}
	else
	{
		cout << "The search has an unexpected error!" << endl;
	}
	SQLFreeStmt(x.stmt, SQL_DROP);
}
void administrator::AddUser(Connect& x)
{
	
	string CID, Cname, Cpassword;
	int CSex;
	SQLWCHAR t[MAX_LEN];
	cout << "Please enter the ID of a customer" << endl;
	cin >> CID; 
	while (CID.length() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> CID;
	}
	cout << "Please enter the name of a customer" << endl;
	cin >> Cname; 
	while (Cname.length() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> Cname;
	}
	cout << "Please enter the password" << endl;
	cin >> Cpassword; 
	while (Cpassword.length() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> Cpassword;
	}
	cout << "Please enter the sex of a customer.(0:man,1:woman)" << endl;
	cin >> CSex;
	while (!(CSex == 0 || CSex == 1))
	{
		cout << "Invalid input.Please try again." << endl;
		cin >> CSex;
	}
    
	string s = "insert into customer values (";
	if (CSex == 0)
		s += ("'" + CID + "','" + Cname + "','" + Cpassword + "','" + "man');");
	else if (CSex == 1)
		s += ("'" + CID + "','" + Cname + "','" + Cpassword + "','" + "woman');");
	swprintf(t, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);//insert into customer.
	if (x.error != SQL_SUCCESS)
	{
		cout << "Add user failed. This ID has existed!" << endl;
	}
	else
	{
		cout << "Add user success!" << endl;
	}
}
void administrator::DropUser(Connect& x)
{
	string delID, s;
	SQLWCHAR tmp[MAX_LEN];
	cout << "Please enter the customer_id you want to delete from the table." << endl;
	cin >> delID;
	s = "select*from customer where customer_id='";
	s = s + delID + "';";
	swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Search
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		if (SQLFetch(x.stmt) == SQL_SUCCESS)
			//Exist-->Delete.
		{
			SQLFreeStmt(x.stmt, SQL_DROP);
			s = "delete from customer where customer_id='" + delID + "';";
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
			if (x.error != SQL_SUCCESS) {
				cout << "Statement handle allocation failed." << endl;
				return;
			}
			x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Del
			if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
			{
				cout << "Delete a user success!" << endl;
			}
			else
			{
				cout << "The process of deleting a user has an unexpected error!" << endl;
			}
			SQLFreeStmt(x.stmt, SQL_DROP);
		}
		else
			//Not exist.
		{
			cout << "This ID does not exist in the database." << endl;
			SQLFreeStmt(x.stmt, SQL_DROP);
		}
	}
	else
	{
		cout << "The search has an unexpected error!" << endl;
		SQLFreeStmt(x.stmt, SQL_DROP);
	}
}
void administrator::ShowBookInf(Connect& x) {
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	string s = "select*from book;";
	SQLWCHAR t[MAX_LEN];
	swprintf(t, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		short i, k = 1;
		char Temp[Column_Book][51];
		char Res[MAX_LEN][Column_Book][51];//store the result.
		SQLINTEGER Maxlength[Column_Book];//max-width with corresponding column.The
		SQLINTEGER TL[Column_Book];
		//Intialize
		Maxlength[0] = strlen("book_id"); strcpy(Res[0][0], "book_id");
		Maxlength[1] = strlen("title"); strcpy(Res[0][1], "title");
		Maxlength[2] = strlen("author"); strcpy(Res[0][2], "author");
		Maxlength[3] = strlen("edition"); strcpy(Res[0][3], "edition");
		Maxlength[4] = strlen("year"); strcpy(Res[0][4], "year");
		Maxlength[5] = strlen("price"); strcpy(Res[0][5], "price");
		//bind
		for (i = 1; i <= 6; i++)
		{
			SQLBindCol(x.stmt, i, SQL_C_CHAR, Temp[i - 1], MAX_LEN, &TL[i - 1]);
		}

		while (SQLFetch(x.stmt) == SQL_SUCCESS)
		{
			for (i = 0; i < 6; i++)
			{
				if (TL[i] == -1)//NULL
				{
					strcpy(Temp[i], "NULL");
				}
				strcpy(Res[k][i], Temp[i]);
				if (strlen(Temp[i]) > Maxlength[i])
				{
					Maxlength[i] = strlen(Temp[i]);//Update max.
				}
			}
			k++;
		}
		//First Line
		printf("+");
		for (i = 0; i < 6; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Second line
		printf("|");
		for (i = 0; i < 6; i++)
		{
			printf("%s", Res[0][i]); //Attribute.
			int Remain = Maxlength[i] - strlen(Res[0][i]);
			for (int j = 0; j < Remain; j++)
			{
				printf(" ");
			}
			printf("|");
		}
		printf("\n");
		//3-rd Line
		printf("+");
		for (i = 0; i < 6; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Data
		for (int j = 1; j < k; j++)
		{
			printf("|");
			for (i = 0; i < 6; i++)
			{
				printf("%s", Res[j][i]);
				int Remain = Maxlength[i] - strlen(Res[j][i]);
				for (int l = 0; l < Remain; l++)
				{
					printf(" ");
				}
				printf("|");
			}
			printf("\n");
		}
		//Final line
		printf("+");
		for (i = 0; i < Column_Book; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
	}
	else
	{
		cout << "The search has an unexpected error!" << endl;
	}
	SQLFreeStmt(x.stmt, SQL_DROP);
}
void administrator::Purchase(Connect& x)
{
	cout << "Please enter the number of books we are going to purchase!" << endl;
	int n;
	cin >> n;
	while (n <= 0)
	{
		cout << "The number of books must be a positive number.Please try again." << endl;
		cin >> n;
	}
	for (int i = 0; i < n; i++) {
		cout << "Turn " << (i + 1) << ":";
		string BID, Btle,Bathr;
		int edition, year; 
		float price;
		SQLWCHAR t[MAX_LEN];
		cout << "Please enter the book ID." << endl;
		cin >> BID;
		while (BID.length() > 20)
		{
			cout << "The length is too long.Please try again." << endl;
			cin >> BID;
		}
		cout << "Please enter the book title." << endl;
		cin >> Btle;
		while (Btle.length() > 50)
		{
			cout << "The length is too long.Please try again." << endl;
			cin >> Btle;
		}
		cout << "Please enter the author of the book." << endl;
		cin >> Bathr;
		while (Bathr.length() > 50)
		{
			cout << "The length is too long.Please try again." << endl;
			cin >> Bathr;
		}
		cout << "Please enter the edition of the book." << endl;
		cin >> edition;
		cout << "Please enter the publish year of the book." << endl;
		cin >> year;
		cout << "Please enter the price of the book." << endl;
		cin >> price;
 
		string s = "insert into book values (";
		s += ("'" + BID + "','" + Btle + "','" + Bathr + "'," + to_string(edition) + "," + to_string(year) + "," + to_string(price) + ");");
		//printf("%s\n", s.c_str());
		swprintf(t, MAX_LEN, L"%hs", s.c_str());
		x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
		if (x.error != SQL_SUCCESS) {
			cout << "Statement handle allocation failed." << endl;
		}
		x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);//insert into book.
		if (x.error != SQL_SUCCESS)
		{
			cout << "Purchase book failed. This book ID has existed!" << endl;
		}
		else
		{
			cout << "Purchase book success!" << endl;
		}
		SQLFreeStmt(x.stmt, SQL_DROP);
	}
}
void administrator::ClearInventory(Connect& x)
{
	cout << "Please enter the number of books you want to clear." << endl;
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		string delID, s;
		SQLWCHAR tmp[MAX_LEN];
		cout << "Please enter the book_id you want to delete." << endl;
		cin >> delID;
		s = "select*from book where book_id='";
		s = s + delID + "';";
		swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
		x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
		if (x.error != SQL_SUCCESS) {
			cout << "Statement handle allocation failed." << endl;
			return;
		}
		x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Search
		if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
		{
			if (SQLFetch(x.stmt) == SQL_SUCCESS)
				//Exist-->Delete.
			{
				SQLFreeStmt(x.stmt, SQL_DROP);
				s = "delete from book where book_id='" + delID + "';";
				swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
				x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
				if (x.error != SQL_SUCCESS) {
					cout << "Statement handle allocation failed." << endl;
					return;
				}
				x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Del
				if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
				{
					cout << "Clear a book inventory success!" << endl;
				}
				else
				{
					cout << "The process of clearing a book inventory has an unexpected error!" << endl;
				}
				SQLFreeStmt(x.stmt, SQL_DROP);
			}
			else
				//Not exist.
			{
				cout << "This book ID does not exist." << endl;
				SQLFreeStmt(x.stmt, SQL_DROP);
			}
		}
		else
		{
			cout << "The search has an unexpected error!" << endl;
			SQLFreeStmt(x.stmt, SQL_DROP);
		}
	}
}
void administrator::ShowAdminInf(Connect& x)//Do not show password of the other administrator.
{
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	string s = "select admin_id,admin_name from administrator;";
	SQLWCHAR t[MAX_LEN];
	swprintf(t, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		short i, k = 1;
		char Temp[2][31];//2 attrbutes.
		char Res[MAX_LEN][2][31];//store the result.
		SQLINTEGER Maxlength[2];//max-width with corresponding column.
		SQLINTEGER TL[2];
		//Intialize
		Maxlength[0] = strlen("admin_id"); strcpy(Res[0][0], "admin_id");
		Maxlength[1] = strlen("admin_name"); strcpy(Res[0][1], "admin_name");
		//bind
		for (i = 1; i <= 2; i++)
		{
			SQLBindCol(x.stmt, i, SQL_C_CHAR, Temp[i - 1], MAX_LEN, &TL[i - 1]);
		}

		while (SQLFetch(x.stmt) == SQL_SUCCESS)
		{
			for (i = 0; i < 2; i++)
			{
				if (TL[i] == -1)//NULL
				{
					strcpy(Temp[i], "NULL");
				}
				strcpy(Res[k][i], Temp[i]);
				if (strlen(Temp[i]) > Maxlength[i])
				{
					Maxlength[i] = strlen(Temp[i]);//Update max.
				}
			}
			k++;
		}
		//First Line
		printf("+");
		for (i = 0; i < 2; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Second line
		printf("|");
		for (i = 0; i < 2; i++)
		{
			printf("%s", Res[0][i]); //Attribute.
			int Remain = Maxlength[i] - strlen(Res[0][i]);
			for (int j = 0; j < Remain; j++)
			{
				printf(" ");
			}
			printf("|");
		}
		printf("\n");
		//3-rd Line
		printf("+");
		for (i = 0; i < 2; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Data
		for (int j = 1; j < k; j++)
		{
			printf("|");
			for (i = 0; i < 2; i++)
			{
				printf("%s", Res[j][i]);
				int Remain = Maxlength[i] - strlen(Res[j][i]);
				for (int l = 0; l < Remain; l++)
				{
					printf(" ");
				}
				printf("|");
			}
			printf("\n");
		}
		//Final line
		printf("+");
		for (i = 0; i < 2; i++)//2 turns
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
	}
	else
	{
		cout << "The search has an unexpected error!" << endl;
	}
	SQLFreeStmt(x.stmt, SQL_DROP);
}
void administrator::AddAdmin(Connect& x)
{
	string CID, Cname, Cpassword;
	SQLWCHAR t[MAX_LEN];
	cout << "Please enter the ID of an admin." << endl;
	cin >> CID;
	while (CID.length() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> CID;
	}
	cout << "Please enter the name of an admin." << endl;
	cin >> Cname;
	while (Cname.length() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> Cname;
	}
	cout << "Please enter the password" << endl;
	cin >> Cpassword;
	while (Cpassword.length() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> Cpassword;
	}

	string s = "insert into administrator values (";
	s += ("'" + CID + "','" + Cname + "','" + Cpassword + "');");
	swprintf(t, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);//insert into admin.
	if (x.error != SQL_SUCCESS)
	{
		cout << "Add an administrator failed. This ID has existed!" << endl;
	}
	else
	{
		cout << "Add an administrator success!" << endl;
	}
}
void administrator::DropAdmin(Connect& x)
{
	string delID, s;
	SQLWCHAR tmp[MAX_LEN];
	cout << "Please enter the admin_id you want to delete from the table." << endl;
	cin >> delID;
	s = "select*from administrator where admin_id='";
	s = s + delID + "';";
	swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Search
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		if (SQLFetch(x.stmt) == SQL_SUCCESS)
			//ID:Exist-->Delete.
		{
			SQLFreeStmt(x.stmt, SQL_DROP);

			s = "select count(distinct admin_id) from administrator;";
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
			if (x.error != SQL_SUCCESS) {
				cout << "Statement handle allocation failed." << endl;
				return;
			}
			x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
			if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
			{
				//If there is only one administrator, it cannot be deleted!
				int Num, length;
				SQLBindCol(x.stmt, 1, SQL_C_LONG, &Num, 8, (SQLINTEGER*)&length);
				if (SQLFetch(x.stmt) == SQL_SUCCESS)//Find
				{
					if (Num > 1)
						//delete permitted.
					{
						SQLFreeStmt(x.stmt, SQL_DROP);
						s = "delete from administrator where admin_id='" + delID + "';";
						swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
						x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
						if (x.error != SQL_SUCCESS) {
							cout << "Statement handle allocation failed." << endl;
							return;
						}
						x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Del
						if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
						{
							cout << "Delete an administrator success!" << endl;
						}
						else
						{
							cout << "The process of deleting an administrator has an unexpected error!" << endl;
						}
						SQLFreeStmt(x.stmt, SQL_DROP);
					}
					else
					{
						cout << "There is only one administrator, which cannot be deleted!" << endl;
					}

				}
				else
				{
					cout << "The process of deleting an administrator has an unexpected error!" << endl;
				}

			}
			else
			{
				cout << "The process of deleting an administrator has an unexpected error!" << endl;
				SQLFreeStmt(x.stmt, SQL_DROP);
			}			
		}
		else
			//Not exist.
		{
			cout << "This ID does not exist in the database." << endl;
			SQLFreeStmt(x.stmt, SQL_DROP);
		}
	}
	else
	{
		cout << "The search has an unexpected error!" << endl;
		SQLFreeStmt(x.stmt, SQL_DROP);
	}
}
void administrator::ShowBorrowInf(Connect& x)
{
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	string s = "select*from borrow;";
	SQLWCHAR t[MAX_LEN];
	swprintf(t, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		short i, k = 1;
		char Temp[4][31];//4 attrbutes.
		char Res[MAX_LEN][4][31];//store the result.
		SQLINTEGER Maxlength[4];//max-width with corresponding column.
		SQLINTEGER TL[4];
		//Intialize
		Maxlength[0] = strlen("customer_id"); strcpy(Res[0][0], "customer_id");
		Maxlength[1] = strlen("book_id"); strcpy(Res[0][1], "book_id");
		Maxlength[2] = strlen("borrow_date"); strcpy(Res[0][2], "borrow_date");
		Maxlength[3] = strlen("return_date"); strcpy(Res[0][3], "return_date");
		//bind
		for (i = 1; i <= 4; i++)
		{
			SQLBindCol(x.stmt, i, SQL_C_CHAR, Temp[i - 1], MAX_LEN, &TL[i - 1]);
		}

		while (SQLFetch(x.stmt) == SQL_SUCCESS)
		{
			for (i = 0; i < 4; i++)
			{
				if (TL[i] == -1)//NULL
				{
					strcpy(Temp[i], "NULL");
				}
				strcpy(Res[k][i], Temp[i]);
				if (strlen(Temp[i]) > Maxlength[i])
				{
					Maxlength[i] = strlen(Temp[i]);//Update max.
				}
			}
			k++;
		}
		//First Line
		printf("+");
		for (i = 0; i < 4; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Second line
		printf("|");
		for (i = 0; i < 4; i++)
		{
			printf("%s", Res[0][i]); //Attribute.
			int Remain = Maxlength[i] - strlen(Res[0][i]);
			for (int j = 0; j < Remain; j++)
			{
				printf(" ");
			}
			printf("|");
		}
		printf("\n");
		//3-rd Line
		printf("+");
		for (i = 0; i < 4; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
		//Data
		for (int j = 1; j < k; j++)
		{
			printf("|");
			for (i = 0; i < 4; i++)
			{
				printf("%s", Res[j][i]);
				int Remain = Maxlength[i] - strlen(Res[j][i]);
				for (int l = 0; l < Remain; l++)
				{
					printf(" ");
				}
				printf("|");
			}
			printf("\n");
		}
		//Final line
		printf("+");
		for (i = 0; i < 4; i++)
		{
			for (int j = 0; j < Maxlength[i]; j++)
			{
				printf("-");
			}
			printf("+");
		}
		printf("\n");
	}
	else
	{
		cout << "The search has an unexpected error!" << endl;
	}
	SQLFreeStmt(x.stmt, SQL_DROP);
}
void administrator::ClearBorrowInf(Connect& x)
{
	cout << "Are you sure you want to delete the extra information in the 'borrow' table" << endl;
	cout << "0.No;1.Yes" << endl;
	string BOOL;
	while (1)
	{
		cin >> BOOL;
		if (BOOL == "0")
		{
			return;
		}
		else if (BOOL == "1")

		{
			break;
		}
		else
		{
			cout << "Invalid input.Please try again!" << endl;
		}
	}
	string	s = "delete from borrow where return_date is not null;";
	SQLWCHAR tmp[MAX_LEN];
	swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Del
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		cout << "Delete the extra information success!" << endl;
	}
	else
	{
		cout << "The process of deleting extra information has an unexpected error!" << endl;
	}
	SQLFreeStmt(x.stmt, SQL_DROP);
}