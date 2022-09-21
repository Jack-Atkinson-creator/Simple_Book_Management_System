#define _CRT_SECURE_NO_WARNINGS
#include"customer.h"
void customer::Register(Connect& x)
{
	string ID, name, password,phone_number,location;
	int sex; //What you need to input.
	cout << "Please input your ID.(No more than 20 words)" << endl;
	cin >> ID;
	while (ID.size()>20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> ID;
	}
	cout << "Please input your name.(No more than 20 words)" << endl;
	cin >> name;
	while (name.size() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> name;
	}
	cout << "Please input your password.(No more than 20 words)" << endl;
	cin >> password;
	while (password.size() > 20)
	{
		cout << "The length is too long.Please try again." << endl;
		cin >> password;
	}
	cout << "Please input your sex.(0:man;1:woman)" << endl;
	cin >> sex;
	while (!(sex == 0 || sex == 1))
	{
		cout << "Invalid input.Please try again." << endl;
		cin >> sex;
	}
	string s = "insert into customer values (";
	if (sex == 0)
		s += ("'" + ID + "','" + name + "','" + password + "','" + "man');");
	else if(sex==1)
		s += ("'" + ID + "','" + name + "','" + password + "','" + "woman');");
	//printf("%s\n", s.c_str());
	SQLWCHAR t[MAX_LEN];
	swprintf(t, MAX_LEN, L"%hs", s.c_str());
	x.error=SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;			
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t,SQL_NTS);//insert into customer.
	if (x.error != SQL_SUCCESS)
	{
		cout << "Register failed. This ID has existed!" << endl;
		return;
	}
	else
	{
		cout << "Register success!" << endl;
	}
	cout << "Please input your phone number.(End with #)" << endl;
	cin >> phone_number;
	while (phone_number != "#")
	{
		if (phone_number.length() != 11)//Not a normal phone number.
		{
			cout << "It's not a correct phone number.Please try again!" << endl;
			cin >> phone_number;
			continue;
		}
		else
		{
			cout << "Please input the location of the phone number.(No more than 30 words)" << endl;
			cin >> location;
			while (location.length() > 30)
			{
				cout << "The length is too long.Please try again." << endl;
				cin >> location;
			}
			//insert 
			s.clear();
			s = "insert into customer_phone values (";
			s += "'" + phone_number + "','" + location + "','"+ID+"');";
			swprintf(t, MAX_LEN, L"%hs", s.c_str());
			x.error= SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);
			if (x.error != SQL_SUCCESS)
			{
				cout << "The phone number has existed.Add phone number failed!" << endl;
			}
		}
		cout << "Please input your phone number.(End with #)" << endl;
		cin >> phone_number;
	}
	SQLFreeStmt(&x.stmt,SQL_DROP);//Free  a statement handle.
}
void customer::LogIn(Connect& x)
{
	cout << "Please input your ID" << endl;
	string id, pass;
    SQLWCHAR tmp[MAX_LEN];//Transition, type conversion.
	cin >> id;
	cout << "Please input your password" << endl;
	cin >> pass;
	string s = "select customer_password from customer where customer_id='";
	s += (id + "';");
	swprintf(tmp, MAX_LEN, L"%hs", s.c_str());

	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
	if (x.error == SQL_SUCCESS)
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
			if (pass == right)
			{
				cout << "Log In success!" << endl;
				CurrID = id;
				CurrPassword = pass;
				//Record the current id and password.
				cout << "What do you want to do?" << endl;
				cout << "1.Search for a book.\n2.Borrow a book.\n3.Return a book.\n4.Update information.\n5.Show Library card.\n0.Exit" << endl;
				string DoSth;
				cin >> DoSth;
				while (1) {
					if (DoSth == "1")
					{
						SearchBook(x);
					}
					else if (DoSth == "2")
					{
						BorrowBook(x);
					}
					else if (DoSth == "3")
					{
						ReturnBook(x);
					}
					else if (DoSth == "4")
					{
						UpdateInfo(x);
					}
					else if (DoSth == "5")
					{
						ShowLibCard(x);
					}
					else if (DoSth == "0")
					{
						break;
					}
					else
					{
						cout << "Invalid input.Please try again." << endl;
					}
					cout << "What do you want to do?" << endl;
					cout << "1.Search for a book.\n2.Borrow a book.\n3.Return a book.\n4.Update information.\n5.Show Library card.\n0.Exit" << endl;
					cin >> DoSth;
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
void customer::ReturnBook(Connect& x)
{
	cout << "Please input the book_id you return." << endl;
	string b_id; 
	SQLWCHAR tmp[MAX_LEN];
	cin >> b_id;
	string s = "select*from borrow where customer_id='";
	s += (CurrID + "' and book_id='" + b_id + "' and return_date is null;");
	swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
	if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
	{
		if (SQLFetch(x.stmt) != SQL_SUCCESS)//Not found
		{
			cout << "The book has been returned,\nor you haven't borrowed this book,\nor the book has been cleared from stock" << endl;
			SQLFreeStmt(x.stmt, SQL_DROP);//Free the statement handle.
		}
		else
		{
			SQLFreeStmt(x.stmt, SQL_DROP);//Free the statement handle.
			x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
			if (x.error != SQL_SUCCESS) {
				cout << "Statement handle allocation failed." << endl;
				return;
			}
			s = "update borrow set return_date='";
			tm* Currtime;
			time_t NowTime;
			time(&NowTime);
			Currtime = localtime(&NowTime);//Get the current date.
			s += (to_string(Currtime->tm_year + 1900) + "-" + to_string(Currtime->tm_mon + 1) + "-" + to_string(Currtime->tm_mday) + "' where book_id='");
			s += (b_id + "'");
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
			if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
			{
				cout << "Return book success!" << endl;
			}
			else
			{
				cout << "The return process has an unexpected error!" << endl;
			}
		}
	}
	else 
	{
		SQLFreeStmt(x.stmt, SQL_DROP);//Free the statement handle.
		cout << "The search for specific book has an unexpected error!" << endl;
	}
}
void customer::BorrowBook(Connect& x)
{
	cout << "Please select attributes you need" << endl;
	cout << "1.book_id;2.Title&&Author&&Year&&Edition;0.exit" << endl;
	string input,b_id,title,author;
	int year, edition;
	//Transition, type conversion.
	SQLWCHAR tmp[MAX_LEN];
	cin >> input;
	while (input != "0")
	{
		if (input == "1")
		{
			cout << "Please input the book_id you need." << endl;
			cin >> b_id;
			string s = "select*from book where book_id='";
			s += (b_id + "' and book_id not in(select book_id from borrow where return_date is null);");
			//The book that is borrowed but nit returned will have the attribute "return_date" null
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			//If there are two or more identical books,we only print the first one.

			x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
			if (x.error != SQL_SUCCESS) {
				cout << "Statement handle allocation failed." << endl;
				return;
			}
			x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
			if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
			{
				char Qbid[21], Qtitle[51], Qauthor[51];
				int Qedition, Qyear, Qlen;
				float Qprice;
				SQLBindCol(x.stmt, 1, SQL_C_CHAR, Qbid, 21, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 2, SQL_C_CHAR, Qtitle, 51, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 3, SQL_C_CHAR, Qauthor, 51, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 4, SQL_C_LONG, &Qedition, 8, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 5, SQL_C_LONG, &Qyear, 8, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 6, SQL_C_FLOAT, &Qprice, 8, (SQLINTEGER*)&Qlen);
				if (SQLFetch(x.stmt) != SQL_SUCCESS)//not found
				{
					cout << "Sorry,the book you are looking for is temporarily not available!" << endl;
					SQLFreeStmt(x.stmt, SQL_DROP);//Free the statement handle.
				}
				else
				{
					printf("book_id:%s;title:%s;author=%s;edition=%d;published in %d;Price is %.2f\n", Qbid, Qtitle, Qauthor, Qedition, Qyear, Qprice);
					SQLFreeStmt(x.stmt, SQL_DROP);//Free the statement handle.
					cout << "Make sure if this book is what you want:0-No,1-Yes" << endl;
					string t;
					cin >> t;
					while (1)
					{
						if (t == "1")//Borrow book.
						{				
							tm* Currtime;
							time_t NowTime;
							time(&NowTime);
							Currtime = localtime(&NowTime);//Get the current date.
							//Borrow the book
							string s = "insert into borrow values('";
							s += (CurrID + "','" + b_id + "','" + to_string(Currtime->tm_year + 1900) + "-" + to_string(Currtime->tm_mon + 1) + "-" + to_string(Currtime->tm_mday) + "',null);");
							x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
							if (x.error != SQL_SUCCESS) {
								cout << "Statement handle allocation failed." << endl;
								return;
							}
							swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
							x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
							if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
							{
								cout << "Borrow book success!" << endl;
							}
							else
							{
								cout << "The borrow process has an unexpected error!" << endl;
							}
							break;
						}
						else if (t == "0")
						{
							break;
						}
						else
						{
							cout << "Invalid inut.Please try again." << endl;
						}
						cin >> t;
					}
				}
			}
			else
			{
				cout << "The search has an unexpected error!" << endl;
			}
		}
		else if (input == "2")
		{
			cout << "Please input the book title you need." << endl;
			cin >>title;
			cout << "Please input the book author you need." << endl;
			cin >> author;
			cout << "Please input the book edition you need." << endl;
			cin >>edition;
			cout << "Please input the year of publishment you need." << endl;
			cin >> year;
			string s = "select*from book where title='";
			s += (title + "' and author='" + author + "' and edition=" + to_string(edition) + " and year=" + to_string(year));
			s += (" and book_id not in(select book_id from borrow where return_date is null);");
			//printf("%s\n", s.c_str());
			//The book that is borrowed but nit returned will have the attribute "return_date" null
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			//If there are two or more identical books,we only print the first one.
			x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
			if (x.error != SQL_SUCCESS) {
				cout << "Statement handle allocation failed." << endl;
				return;
			}
			x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
			if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
			{
				char Qbid[21], Qtitle[51], Qauthor[51];
				int Qedition, Qyear, Qlen;
				float Qprice;
				SQLBindCol(x.stmt, 1, SQL_C_CHAR, Qbid, 21, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 2, SQL_C_CHAR, Qtitle, 51, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 3, SQL_C_CHAR, Qauthor, 51, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 4, SQL_C_LONG, &Qedition, 8, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 5, SQL_C_LONG, &Qyear, 8, (SQLINTEGER*)&Qlen);
				SQLBindCol(x.stmt, 6, SQL_C_FLOAT, &Qprice, 8, (SQLINTEGER*)&Qlen);
				if (SQLFetch(x.stmt) != SQL_SUCCESS)//not found
				{
					cout << "Sorry,the book you are looking for is temporarily not available!" << endl;
					SQLFreeStmt(x.stmt, SQL_DROP);//Free the statement handle.
				}
				else
				{
					printf("book_id:%s;title:%s;author=%s;edition=%d;published in %d;Price is %.2f\n", Qbid, Qtitle, Qauthor, Qedition, Qyear, Qprice);
					SQLFreeStmt(x.stmt, SQL_DROP);//Free the statement handle.
					cout << "Make sure if this book is what you want:0-No,1-Yes" << endl;
					string t;
					cin >> t;
					while (1)
					{
						if (t == "1")//Borrow book.
						{
							tm* Currtime;
							time_t NowTime;
							time(&NowTime);
							Currtime = localtime(&NowTime);//Get the current date.
							//Borrow the book
							string s = "insert into borrow values('";
							string s2(Qbid);//book_id with string pattern.
							s += (CurrID + "','" + s2 + "','" + to_string(Currtime->tm_year+1900) + "-" + to_string(Currtime->tm_mon+1) + "-" + to_string(Currtime->tm_mday) + "',null);");
							x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
							if (x.error != SQL_SUCCESS) {
								cout << "Statement handle allocation failed." << endl;
								return;
							}
							swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
							//printf("%ws", tmp);
							x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
							if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
							{
								cout << "Borrow book success!" << endl;
							}
							else
							{
								cout << "The borrow process has an unexpected error!" << endl;
							}
							break;
						}
						else if (t == "0")//Cancel borrowing.
						{
							break;
						}
						else
						{
							cout << "Invalid inut.Please try again." << endl;
						}
						cin >> t;
					}
				}
			}
			else
			{
				cout << "The search has an unexpected error!" << endl;
			}
		}
		else
		{
			cout << "Invalid input.Please try again." << endl;
		}
		cout << "Please select attributes you need." << endl;
		cout << "1.book_id;2.Title&&Author&&Year&&Edition;0.exit" << endl;
		cin >> input;
	}

}
void customer::SearchBook(Connect& x)
//Multiple identical books have different book_id.
{
	cout << "Search by:" << endl;
	//Fuzzy search!
	cout << "1.book_id,2.title,3.author,4.edition,5.year,0.Exit" << endl;
	string searchMethod;
	//Transition, type conversion.
	SQLWCHAR tmp[MAX_LEN];
	cin >> searchMethod;
	while (searchMethod!="0")
	{
		if (searchMethod == "1")
			//Search by book_id
		{
			cout << "Enter the book_id that you want to find." << endl;
			string bid;
			cin >> bid;
			string s = "select* from book where book_id like '%";
			s += (bid + "%';");
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			PrintBookInfo(x, tmp);
		}
		else if (searchMethod == "2")
			//Search by title
		{
			cout << "Enter the book title that you want to find." << endl;
			string btt;
			cin >> btt;
			string s = "select* from book where title like '%";
			s += (btt + "%';");
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			PrintBookInfo(x, tmp);
		}
		else if (searchMethod == "3")
			//Search by author
		{
			cout << "Enter the author of books that you want to find." << endl;
			string ba;
			cin >> ba;
			string s = "select* from book where author like '%";
			s += (ba + "%';");
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			PrintBookInfo(x, tmp);
		}
		else if (searchMethod == "4")
			//Search by edition
		{
			cout << "Enter the edition of books that you want to find." << endl;
			string be;
			cin >> be;
			string s = "select* from book where edition=";
			s += (be + ";");
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			PrintBookInfo(x, tmp);
		}
		else if (searchMethod == "5")
			//Search by year
		{
			cout << "Enter the year of books that you want to find." << endl;
			string by;
			cin >> by;
			string s = "select* from book where year=";
			s += (by + ";");
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			PrintBookInfo(x, tmp);
		}
		else
		{
			cout << "Invalid input.Please try again." << endl;
		}
		cout << "Search by:" << endl;
		cout << "1.book_id,2.title,3.author,4.edition,5.year,0.Exit" << endl;	
		cin >> searchMethod;
	}

}
void customer::Run(Connect& x)
{
	while (1)
	{
		string input;
		cout << "1.Register;2.LogIn;0.Exit"<<endl;
		cin >> input;
		if (input == "1")
		{
			Register(x);
		}
		else if (input == "2")
		{
			LogIn(x);
		}
		else if(input=="0")
		{
			break;//exit
		}
		else
		{
			cout << "Invalid input.Please try again." << endl;
		}
	}
}
void customer::PrintBookInfo(Connect& x, SQLWCHAR* t)
{
	//printf("%ws\n", t);
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)t, SQL_NTS);
	if (x.error == SQL_SUCCESS|| x.error==SQL_SUCCESS_WITH_INFO)
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
void customer::UpdateInfo(Connect& x)
{
	cout << "Please choose the type of information you want to update" << endl;
	cout << "1.password\n2.name\n3.sex\n4.phone_number\n0.exit" << endl;
	string input;
	SQLWCHAR tmp[MAX_LEN];
	cin >> input;
	while (input != "0")//0:exit
	{
		if (input == "1")		//change password.
		{
			cout << "Please enter your old password." << endl;
			string tmpass, newpass;
			cin >> tmpass;
			if (tmpass != CurrPassword) //old password nor correct.
			{
				cout << "Password is not correct." << endl;
			}
			else //password is correct.
			{
				cout << "Please enter your new password." << endl;
				cin >> newpass;
				string s = "update customer set customer_password='";
				s += (newpass + "' where customer_id='" + CurrID + "';");
				swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
				x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
				if (x.error != SQL_SUCCESS) {
					cout << "Statement handle allocation failed." << endl;
					return;
				}
				x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
				if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
					//Success.
				{
					cout << "Update password success!" << endl;
					CurrPassword = newpass;
				}
				else
				{
					cout << "The search has an unexpected error!" << endl;
				}
				SQLFreeStmt(x.stmt, SQL_DROP);
			}
		}
		else if (input == "2")//change name
		{
			cout << "Please enter your new name" << endl;
			string name;
			cin >> name;
			string s = "update customer set customer_name='";
			s += (name + "' where customer_id='" + CurrID + "';");
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
			if (x.error != SQL_SUCCESS) {
				cout << "Statement handle allocation failed." << endl;
				return;
			}
			x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
			if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
				//Success.
			{
				cout << "Update name success!" << endl;
			}
			else
			{
				cout << "The search has an unexpected error!" << endl;
			}
			SQLFreeStmt(x.stmt, SQL_DROP);
		}
		else if (input == "3")//change sex
		{
			cout << "Please enter your new sex.(0:man;1:woman)" << endl;
			string nsex;
			cin >> nsex;
			string s = "update customer set customer_sex=";
			//s += (name + "' where customer_id='" + CurrID + "';");
			if (nsex == "0")
			{
				s += ("'man' where customer_id='" + CurrID + "';");
			}
			else if (nsex == "1")
			{
				s += ("'woman' where customer_id='" + CurrID + "';");
			}
			else
			{
				cout << "Invalid input.Please try again." << endl;
				goto Loop;
			}
			swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
			x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
			if (x.error != SQL_SUCCESS) {
				cout << "Statement handle allocation failed." << endl;
				return;
			}
			x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
			if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
				//Success.
			{
				cout << "Update sex success!" << endl;
			}
			else
			{
				cout << "The search has an unexpected error!" << endl;
			}
		   SQLFreeStmt(x.stmt, SQL_DROP);
	   Loop:;
		}
		else if (input == "4")//change phone_number
		{
			cout << "Please make your choice." << endl;
			cout << "1.Update phone_number.\n2.Add new phone_number.\n3.Delete phone_number" << endl;
			string choice;
			cin >> choice;
			if (choice == "1")//update
			{
				string oldphone, s;
				cout << "Please enter your old phone_number." << endl;
				cin >> oldphone;
				s = "select customer_id from customer_phone where phone_number='";
				s = s + oldphone + "';";
				swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
				x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
				if (x.error != SQL_SUCCESS) {
					cout << "Statement handle allocation failed." << endl;
					return;
				}
				x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Search
				if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
				{
					char ID[22]; int Size; bool flag = false;
					SQLBindCol(x.stmt, 1, SQL_C_CHAR, ID, MAX_LEN, (SQLINTEGER*)&Size);
					while (SQLFetch(x.stmt) == SQL_SUCCESS)
					{
						string tmpID(ID);
						if (CurrID == ID)
						{
							flag = true;
							break;
						}
					}
					SQLFreeStmt(x.stmt, SQL_DROP);
					if (flag)
						//Find the phone_number.
					{
						string newphone;
						cout << "Please enter your new phone_number." << endl;
						cin >> newphone;
						while (newphone.length() != 11)
						{
							cout << "Invalid phone_number.Please input again." << endl;
							cin >> newphone;
						}
						s = "update customer_phone set phone_number='";
						s = s + newphone + "' where phone_number='" + oldphone + "';";
						swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
						x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
						if (x.error != SQL_SUCCESS) {
							cout << "Statement handle allocation failed." << endl;
							return;
						}
						x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
						if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
						{
							cout << "Update phone_number success!" << endl;
						}
						else
						{
							cout << "The update phone_number process has an unexpected error." << endl;
						}
						SQLFreeStmt(x.stmt, SQL_DROP);
					}
					else
					{
						cout << "The phone_number doesn't exist." << endl;
					}

				}
				else
				{
					cout << "The search has an unexpected error!" << endl;
					SQLFreeStmt(x.stmt, SQL_DROP);
				}
			}
			else if (choice == "2")//add
			{
				string newphone, newaddr, s;
				cout << "Please enter the phone_number you want to add" << endl;
				cin >> newphone;
				s = "select customer_id from customer_phone where phone_number='";
				s = s + newphone + "';";
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
						//Someone use this phone_number.
					{
						cout << "This phone number is already in use" << endl;
						SQLFreeStmt(x.stmt, SQL_DROP);
					}
					else {
						SQLFreeStmt(x.stmt, SQL_DROP);
						cout << "Please enter the location of this new phone number." << endl;
						string loc;
						cin >> loc;
						while (loc.length() > 30)
						{
							cout << "The length is too long.Please try again." << endl;
							cin >> loc;
						}
						s = "insert into customer_phone values('";
						s = s + newphone + "','" + loc + "','" + CurrID + "');";
						swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
						x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
						if (x.error != SQL_SUCCESS) {
							cout << "Statement handle allocation failed." << endl;
							return;
						}
						x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);
						if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
						{
							cout << "Add new phone_number success!" << endl;
						}
						else
						{
							cout << "The adding phone_number process has an unexpected error." << endl;
						}
						SQLFreeStmt(x.stmt, SQL_DROP);
					}
				}
				else
				{
					cout << "The search has an unexpected error!" << endl;
					SQLFreeStmt(x.stmt, SQL_DROP);
				}
			}
			else if (choice == "3")//delete
			{
				string dephone, s;
				cout << "Please enter the phone_number you want to delete" << endl;
				cin >> dephone;
				s = "select customer_id from customer_phone where phone_number='";
				s = s + dephone + "';";
				swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
				x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
				if (x.error != SQL_SUCCESS) {
					cout << "Statement handle allocation failed." << endl;
					return;
				}
				x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Search
				if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
				{
					char cid[21]; int length;
					SQLBindCol(x.stmt, 1, SQL_C_CHAR, cid, 21, (SQLINTEGER*)&length);
					if (SQLFetch(x.stmt) == SQL_SUCCESS)
						//Exist
					{
						string tmpID(cid);
						if (tmpID != CurrID)//Not your phone.
						{
							cout << "You don't have this phone_number." << endl;
							SQLFreeStmt(x.stmt, SQL_DROP);
						}
						else //Your phone.
						{
							SQLFreeStmt(x.stmt, SQL_DROP);
							s = "delete from customer_phone where phone_number='" + dephone + "';";
							swprintf(tmp, MAX_LEN, L"%hs", s.c_str());
							x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
							if (x.error != SQL_SUCCESS) {
								cout << "Statement handle allocation failed." << endl;
								return;
							}
							x.error = SQLExecDirect(x.stmt, (SQLWCHAR*)tmp, SQL_NTS);//Del
							if (x.error == SQL_SUCCESS || x.error == SQL_SUCCESS_WITH_INFO)
							{
								cout << "Delete phone_number success!" << endl;
							}
							else
							{
								cout << "The deleting phone_number process has an unexpected error." << endl;
							}
							SQLFreeStmt(x.stmt, SQL_DROP);
						}
					}
					else
						//Not exist.
					{
						cout << "You don't have this phone_number." << endl;
						SQLFreeStmt(x.stmt, SQL_DROP);
					}
				}
				else
				{
					cout << "The search has an unexpected error!" << endl;
					SQLFreeStmt(x.stmt, SQL_DROP);
				}

			}
			else
			{
				cout << "Invalid input.Please try again." << endl;
			}
		}
		cout << "Please choose the type of information you want to update" << endl;
		cout << "1.password\n2.name\n3.sex\n4.phone_number\n0.exit" << endl;
		cin >> input;
	}
}
void customer::ShowLibCard(Connect& x)
//Show library card of the book that a customer borrows.
{
	x.error = SQLAllocStmt(x.conn, &x.stmt);//Allocate a statement handle.
	if (x.error != SQL_SUCCESS) {
		cout << "Statement handle allocation failed." << endl;
		return;
	}
	string s = "select*from borrow where customer_id='";
	s += (CurrID + "';");
//	printf("%s\n", s.c_str());
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
