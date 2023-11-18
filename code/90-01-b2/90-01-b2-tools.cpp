/* 胡逸凡 信01 2153592 */
#include<iostream>
#include<iomanip>
#include<windows.h>
#include"90-01-b2.h"
#include "../include/cmd_console_tools.h"
#define NUM 20
using namespace std;

int is_end(int arr[8][10], int row, int col)
{
	int i, j, a[10][12], sum, n = 0;
	for (i = 0; i < 12; i++)
	{
		a[0][i] = 0;
		a[row + 1][i] = 0;
	}
	for (i = 0; i < 10; i++)
	{
		a[i][0] = 0;
		a[i][col + 1] = 0;
	}
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			a[i + 1][j + 1] = arr[i][j];
	for (i = 1; i < row + 1; i++)
		for (j = 1; j < col + 1; j++)
		{
			sum = 0;
			if (a[i][j] != a[i + 1][j])
				sum++;
			if (a[i][j] != a[i - 1][j])
				sum++;
			if (a[i][j] != a[i][j + 1])
				sum++;
			if (a[i][j] != a[i][j - 1])
				sum++;
			if (sum == 4)
				n++;
		}
	if (n == col * row)
		return 1;
	else
		return 0;
}

void pre_grade(int sum, int* sum_grade, int t, int dst)
{
	int grade;
	grade = (t - 1) * sum * 3;
	*sum_grade += grade;
	cct_gotoxy(0, 0);
	cout << "本次得分:" << grade << " 总得分:" << *sum_grade << "合成目标:" << dst;
}

void input_init(int* row, int* col, int* dst, char mid[8][10], int z)
{
	cct_cls();
	int r, c, d;
	restore_mid(mid);
	while (1)
	{
		cout << "请输入行数(5-8)：" << endl;
		cin >> r;
		if (cin.good() && r >= 5 && r <= 8)
			break;
		else
		{
			cin.clear();
			cin.ignore(NUM, '\n');
		}
	}
	while (1)
	{
		cout << "请输入列数(5-10)：" << endl;
		cin >> c;
		if (cin.good() && c >= 5 && c <= 10)
			break;
		else
		{
			cin.clear();
			cin.ignore(NUM, '\n');
		}
	}
	*row = r;
	*col = c;
	if (z > 2)
	{
		while (1)
		{
			cout << "请输入合成目标(5-20)：" << endl;
			cin >> d;
			if (cin.good() && d <= 20 && d >= 5)
				break;
			else
			{
				cin.clear();
				cin.ignore(NUM, '\n');
			}
		}
		*dst = d;
	}
	cin.ignore(1024, '\n');
}

void input_tmp(int* num, char* a, int row, int col, int arr[8][10], char mid[8][10], int re)
{
	if (re == 1)
	{
		cout << "输入的矩阵坐标处无连续相同值，请重新输入" << endl;
		mid[*a - 65][*num] = '0';
	}
	int x, y;
	char pa[50];
	cout << "请以字母+数字形式[例：c2]输入矩阵坐标：";
	cct_getxy(x, y);
	while (1)
	{
		cin.getline(pa, 50, '\n');
		cin.clear();
		if (pa[1] >= '0' && pa[1] < col + '0' && ((pa[0] >= 'A' && pa[0] <= 'A' + row - 1) || (pa[0] >= 'a' && pa[0] <= 'a' + row - 1)) && strlen(pa) == 2)
			break;
		else
			cout << "输入错误，请重新输入.";
		cct_gotoxy(x, y);
		cout << setw(20) << ' ';
		cct_gotoxy(x, y);
	}
	*num = pa[1] - '0';
	if (pa[0] >= 'a' && pa[0] <= 'a' + col - 1)
		pa[0] = pa[0] - 32;
	*a = pa[0];
	cout << "输入为" << *a << "行" << *num << "列" << endl;
}

int judge(int num, char a, int arr[8][10], char mid[8][10], int row, int col)
{
	int i, j, p, sum = 0, s;
	p = a - 65;
	mid[p][num] = '*';
	while (1)
	{
		s = sum;
		sum = 0;
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)
			{
				if (arr[i][j] == arr[p][num] && mid[i][j] != '*')
				{
					if ((i < row - 1 && mid[i + 1][j] == '*') || (i > 0 && mid[i - 1][j] == '*') || (j > 0 && mid[i][j - 1] == '*' && j > 0) || (j < col - 1 && mid[i][j + 1] == '*'))
						mid[i][j] = '*';
				}
			}
		sum = num_mine(row, col, mid);
		if (s == sum || sum == 1)
			break;
	}
	if (sum == 1)
		return 1;
	else
		return 0;
}

void find(int x, int y, int arr[8][10], char mid[8][10], int t, int row, int col)//t=arr[i][j]
{
	if (x > 0)
		if (arr[x - 1][y] == t && mid[x - 1][y] != '*')
		{
			mid[x - 1][y] = '*';
			find(x - 1, y, arr, mid, t, row, col);
		}
	if (y > 0)
		if (arr[x][y - 1] == t && mid[x][y - 1] != '*')
		{
			mid[x][y - 1] = '*';
			find(x, y - 1, arr, mid, t, row, col);
		}
	if (x < row - 1)
		if (arr[x + 1][y] == t && mid[x + 1][y] != '*')
		{
			mid[x + 1][y] = '*';
			find(x + 1, y, arr, mid, t, row, col);
		}
	if (y < col - 1)
		if (arr[x][y + 1] == t && mid[x][y + 1] != '*')
		{
			mid[x][y + 1] = '*';
			find(x, y + 1, arr, mid, t, row, col);
		}
}

int recursion_judge(int num, char a, int arr[8][10], char mid[8][10], int row, int col)
{
	int p, sum = 0;
	p = a - 65;
	mid[p][num] = '*';
	find(p, num, arr, mid, arr[p][num], row, col);
	sum = num_mine(row, col, mid);
	if (sum == 1)
		return 1;
	else
		return 0;
}