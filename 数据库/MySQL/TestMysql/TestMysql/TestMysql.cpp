// TestMysql.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<Windows.h>
#include <mysql.h>
#include<string>


#pragma  comment(lib, "../lib/libmysql.lib")


using namespace std;


int main()
{

	//�ر���һ�����ݽṹ
	MYSQL mydata;

	//��ʼ�����ݿ�
	if (0 == mysql_library_init(0, NULL, NULL)) {
		cout << "mysql_library_init() succeed" << endl;
	}
	else {
		cout << "mysql_library_init() failed" << endl;
		return -1;
	}

	//��ʼ�����ݽṹ
	if (NULL != mysql_init(&mydata)) {
		cout << "mysql_init() succeed" << endl;
	}
	else {
		cout << "mysql_init() failed" << endl;
		return -1;
	}

	//���������ݿ�֮ǰ�����ö��������ѡ��
	//�������õ�ѡ��ܶ࣬���������ַ����������޷���������
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() succeed" << endl;
	}
	else {
		cout << "mysql_options() failed" << endl;
		return -1;
	}

	//�������ݿ�
	if (NULL
		!= mysql_real_connect(&mydata, "localhost", "root", "12345678", "test",
			3306, NULL, 0))
		//����ĵ�ַ���û��������룬�˿ڿ��Ը����Լ����ص��������
	{
		cout << "mysql_real_connect() succeed" << endl;
	}
	else {
		cout << "mysql_real_connect() failed" << endl;
		system("pause");
		return -1;
	}

	//sql�ַ���
	string sqlstr;

	//����һ����
	sqlstr = "CREATE TABLE IF NOT EXISTS `new_paper` (";
	sqlstr += " `NewID` int(11) NOT NULL AUTO_INCREMENT,";

	sqlstr += " `NewCaption` varchar(40) NOT NULL,";

	sqlstr += " `NewContent` text,";

	sqlstr += " `NewTime` DATE DEFAULT NULL,";

	sqlstr += " PRIMARY KEY(`NewID`)";

	sqlstr += " ) ENGINE = InnoDB DEFAULT CHARSET = utf8";


	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() create table succeed" << endl;
	}
	else {
		cout << "mysql_query() create table failed" << endl;
		mysql_close(&mydata);
		return -1;
	}


	//����в�������
	for (int i = 0; i < 100; i++)
	{
		sqlstr =
			"INSERT INTO `test`.`new_paper` (`NewID`, `NewCaption`, `NewContent`, `NewTime`) ";
		sqlstr += "VALUES (default, '����', '��һЩ����', NOW());";
		if (0 == mysql_query(&mydata, sqlstr.c_str())) {
			cout << "mysql_query() insert data succeed" << endl;
		}
		else {
			cout << "mysql_query() insert data failed" << endl;
			mysql_close(&mydata);
			return -1;
		}
	}

	//��ʾ�ղŲ��������
	//sqlstr = "SELECT `NewID`,`NewCaption`,`NewContent`,`NewTime` FROM `test`.`new_paper`";
	sqlstr = "SELECT * FROM `test`.`new_paper`";
	MYSQL_RES *result = NULL;
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() select data succeed" << endl;

		//һ����ȡ�����ݼ�
		result = mysql_store_result(&mydata);
		//ȡ�ò���ӡ����
		int rowcount = mysql_num_rows(result);
		cout << "row count: " << rowcount << endl;

		//ȡ�ò���ӡ���ֶε�����
		unsigned int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD *field = NULL;
		for (unsigned int i = 0; i < fieldcount; i++) {
			field = mysql_fetch_field_direct(result, i);
			cout << field->name << "\t\t";
		}
		cout << endl;

		//��ӡ����
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while (NULL != row) {
			for (int i = 0; i < fieldcount; i++) {
				cout << row[i] << "\t\t";
			}
			cout << endl;
			row = mysql_fetch_row(result);
		}

	}
	else {
		cout << "mysql_query() select data failed" << endl;
		mysql_close(&mydata);
		return -1;
	}


#if 1
	//ɾ���ղŽ��ı�
	sqlstr = "DROP TABLE `test`.`new_paper`";
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() drop table succeed" << endl;
	}
	else {
		cout << "mysql_query() drop table failed" << endl;
		mysql_close(&mydata);
		return -1;
	}
#endif

	mysql_free_result(result);
	mysql_close(&mydata);
	mysql_server_end();

	system("pause");
	return 0;
}
