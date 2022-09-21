
#include<Windows.h>
#include<iostream>
#include<string>
#include"Connection.h"
#include"customer.h"
#include"administrator.h"
using namespace std;

int main(int argc,char**argv)
{
    Connect mainwindow;
	customer cust;//temporary object.
	administrator admin;
    bool sts= mainwindow.ConnectWithDB();
    if (sts)//Connection Success
    {
		string inputstr;
		while (1)
		{
			cout << "Please input your identity" << endl;
			cout << "1.Administrator\n2.Customer\n0.exit" << endl;
			cin >> inputstr;
			if (inputstr == "1")
			{
				//Do Administrator until exit.
				admin.Run(mainwindow);
			}
			else if (inputstr == "2")
			{
				//cout << "Do Customer" << endl;
				cust.Run(mainwindow);
			}
			else if (inputstr == "0")
			{
				break;//exit
			}
			else
			{
				cout << "Invalid input.Please try again" << endl;
			}
		}
        mainwindow.DisConnectWithDB();
    }
    return 0;
}

