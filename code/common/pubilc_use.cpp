/* ��01 2153592 ���ݷ� */
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ӡ�������ȸ������ַ�
 ***************************************************************************/
void print_special(const int length, const char p)  //�޻���
{
	int i;
	for (i = 0; i < length; i++)
		cout << p;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����˵�����
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
	cout << "[��ѡ��:]";
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����ȴ����������ַ�������END�ȣ��Դ�Сд
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������鸴ԭ��Ϊ0���������Ϊ8x10
 ***************************************************************************/
void assign_arr(const int row, const int col, int arr[ROW][COL])
{
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			arr[i][j] = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������_maxΪ���������
  �� �� ֵ��
  ˵    ���������������
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ���������������ֵ
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����������鸴ԭ
 ***************************************************************************/
void restore_mid(int mid[ROW][COL])
{
	int i, j;
	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
			mid[i][j] = '0';
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������ص����������
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������������
 ***************************************************************************/
void swap(int* a, int* b)
{
	int t;
	t = *a;
	*a = *b;
	*b = t;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������ַ���תΪint����Ҫ����ip�ȴ�
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