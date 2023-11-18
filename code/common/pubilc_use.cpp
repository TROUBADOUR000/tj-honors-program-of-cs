/* 信01 2153592 胡逸凡 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
using namespace std;

#define ROW 8
#define COL 10
#define MAX 1024

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：打印给定长度个特殊字符
 ***************************************************************************/
void print_special(const int length, const char p)  //无换行
{
	int i;
	for (i = 0; i < length; i++)
		cout << p;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：菜单函数
 ***************************************************************************/
int menu(const char* p, const char* option)
{
	int n, length = 0, i, s = int(strlen(p)), j = 0, mark = 1, x, y, x1, y1;
	const char* flag, * q = p;
	cct_getconsoleborder(x, y, x1, y1);
	while (int(q - p) < s)
	{
		i = 0;
		for (flag = q; *flag != '\n'; flag++)
			i++;
		q = ++flag;
		if (length < i)
			length = i;
	}
	length = length < x ? length : x;
	print_special(length, '-');
	if (length < x)
		cout << endl;
	cout << p;
	print_special(length, '-');
	if (length < x)
		cout << endl;
	cout << "[请选择:]";
	while (1)
	{
		n = _getch();
		for (i = 0; option[i] != '\0'; i++)
			if (n == option[i])
			{
				mark = 0;
				break;
			}
		if (mark == 0)
		{
			cout << char(n) << endl;
			return n;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：等待输入特殊字符串，如END等，对大小写
 ***************************************************************************/
void wait_for_key(const char* wait_key_value, const int X = -1, const int Y = -1)
{
	char prompt[1024];
	while (1)
	{
		cin.getline(prompt, MAX, '\n');
		if (!strcmp(prompt, wait_key_value))
			break;
		cct_gotoxy(X, Y);
		cout << setw(64) << setfill(' ') << ' ';
		cct_gotoxy(X, Y);
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：数组复原置为0，最大数组为8x10
 ***************************************************************************/
void assign_arr(const int row, const int col, int arr[ROW][COL])
{
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			arr[i][j] = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：_max为生成最大数
  返 回 值：
  说    明：数组随机生成
 ***************************************************************************/
void init_arr(const int row, const int col, int arr[ROW][COL],const int _max)
{
	srand((unsigned int)time(NULL));
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			arr[i][j] = rand() % _max + 1;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：求数组中最大值
 ***************************************************************************/
int max_arr(const int row, const int col, int arr[ROW][COL])
{
	int i, j, m = 0;
	for (j = 0; j < col; j++)
		for (i = 0; i < row; i++)
		{
			if (arr[i][j] > m)
				m = arr[i][j];
		}
	return m;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：地雷数组复原
 ***************************************************************************/
void restore_mid(int mid[ROW][COL])
{
	int i, j;
	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
			mid[i][j] = '0';
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：返回地雷数组个数
 ***************************************************************************/
int num_mid(const int row, const int col, int mid[ROW][COL])
{
	int i, j, sum = 0;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			if (mid[i][j] == '*')
				sum++;
	return sum;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：交换两数
 ***************************************************************************/
void swap(int* a, int* b)
{
	int t;
	t = *a;
	*a = *b;
	*b = t;
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：把字符串转为int，主要用于ip等处
 ***************************************************************************/
int char_to_int(char* tmp)
{
	int i, n = 0;
	for (i = 0; tmp[i] != '\0'; i++)
	{
		n *= 10;
		n += tmp[i];
	}
	return n;
}