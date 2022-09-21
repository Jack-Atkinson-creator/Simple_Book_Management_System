# Simple_Book_Management_System

关系模式

customer(*customer_id*,customer_name,customer_password,customer_sex), primary_key=customer_id

customer_phone(*phone_number*,location,customer_id), primary_key=phone_number, foreign key customer_id references customer(customer_id)

book(*book_id*,title,author,edition.year,price), primary_key=book_id

borrow(*customer_id*,*book_id*,borrow_date,return_date) , primary_key=customer_id,book_id; foreign key customer_id references customer(customer_id),foreign key book_id references book(book_id)

administrator(*admin_id*,admin_name.,admin_password), primary_key= admin_id

基本过程：

1.登录MySQL建立数据库，表、主键和外键如上述关系模式

2.MySQL端进行连接

（1）从Windows管理工具中打开“ODBC数据源（64位）”，添加“MySQL ODBC 8.0 ANSI Driver”,在ODBC Data Source Configuration里面输入账号密码等信息。点击test测试是否成功。


![image](https://user-images.githubusercontent.com/71262284/191462682-eed59e8d-53a7-4e4c-b7f0-d1d0a8984df9.png)

（2）在Visual Studio 2022中创建项目，点击“工具--连接到数据库”，按下图输入相关信息。点击测试连接，检测是否连接成功

![image](https://user-images.githubusercontent.com/71262284/191465150-ef24add5-b552-42f3-a6a2-697b425cc0d5.png)


3.打开Visual Studio，选择开始执行（不调试）即可运行。

4.类的设计

（1）管理员类：用于对用户信息、管理员信息进行增删查改，对书籍信息进行显示、购买、清楚库存和修改信息，对借阅记录进行增加和删除。

（2）用户类：用于实现借阅、归还、查询图书信息，也可以对自己的信息进行修改和显示。
