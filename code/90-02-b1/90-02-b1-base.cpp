/* 2153592 信01 胡逸凡 */
#include<iostream>
#include<iomanip>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
using namespace std;

int square(int x, char y, int a[9][9], char fla[9][9])
{
	int copy[9], i, j, k, judge = 1;
	for (i = 0; i < 9;)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++, i++)         //i随k变化
			{
				copy[i] = a[x + j][y - 'a' + k];
				if (copy[i] > 9 || copy[i] < 0)
				{
					judge = 0;
					break;
				}
			}
	if (judge != 0)
		for (i = 0; i < 8; i++)
			for (j = i + 1; j < 9; j++)
				if (copy[i] != 0 && copy[i] == copy[j])
				{
					judge = 2;
					fla[x + i / 3][y - 'a' + i % 3] = '#';
					fla[x + j / 3][y - 'a' + j % 3] = '#';
				}
	if (judge == 2)
	{
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				if (fla[x + i][y - 'a' + j] != '#')
					fla[x + i][y - 'a' + j] = '!';
		judge = 0;
	}
	return judge;
}

int col(char y, int a[9][9], char fla[9][9])
{
	int i, j, judge = 1, u;
	u = y - 'a';
	for (i = 0; i < 8; i++)
	{
		if (a[i][u] > 9 || a[i][u] < 0)
		{
			judge = 0;
			break;
		}
		else
			if (a[i][u] != 0)
				for (j = i + 1; j < 9; j++)
					if (a[i][u] == a[j][u])
					{
						judge = 2;
						fla[i][u] = '#';
						fla[j][u] = '#';
					}
	}
	if (judge == 2)
	{
		for (i = 0; i < 9; i++)
			if (fla[i][u] != '#')
				fla[i][u] = '!';
		judge = 0;
	}
	return judge;
}

int row(int x, int a[9][9], char fla[9][9])
{
	int i, j, judge = 1;
	for (i = 0; i < 8; i++)
	{
		if (a[x][i] > 9 || a[x][i] < 0)
		{
			judge = 0;
			break;
		}
		else
			if (a[x][i] != 0)
				for (j = i + 1; j < 9; j++)
					if (a[x][i] == a[x][j])
					{
						judge = 2;
						judge = 2;
						fla[x][i] = '#';
						fla[x][j] = '#';
					}
	}
	if (judge == 2)
	{
		for (i = 0; i < 9; i++)
			if (fla[x][i] != '#')
				fla[x][i] = '!';
		judge = 0;
	}
	return judge;
}

int is_sudoku(int a[9][9], char fla[9][9])
{
	int i, j, judge = 0;
	for (i = 0; i < 9; i++)
		judge += row(i, a, fla) + col(char(i + 'a'), a, fla);
	for (i = 0; i <= 6; i += 3)
		for (j = 0; j <= 6; j += 3)
			judge += square(i, char(j + 'a'), a, fla);
	return judge == 27 ? 0 : 27 - judge;
}

int is_solution(int a[9][9], char fla[9][9])
{
	int t = 1, i, j, sum;
	if (is_sudoku(a, fla) == 0)
	{
		for (i = 0; i < 9; i++)
		{
			sum = 0;
			for (j = 0; j < 9; j++)
				sum += a[i][j];
			if (sum != 45)
			{
				t = 0;
				break;
			}
		}
		if (t == 1)
			for (j = 0; j < 9; j++)
			{
				sum = 0;
				for (i = 0; i < 9; i++)
					sum += a[i][j];
				if (sum != 45)
				{
					t = 0;
					break;
				}
			}
	}
	else
		t = 0;
	return t;
}

int check(int x, char y, int a[9][9],int n,char flag[9][9], char fla[9][9])
{
	int u, judge = 0;
	u = y - 'a';
	if (flag[x][u] == '*')
		return 9;
	a[x][u] = n;
	judge += (col(y, a, fla) == 1 ? 0 : 1);
	judge += (row(x, a, fla) == 1 ? 0 : 2);
	judge += (square(x / 3 * 3, char(u / 3 * 3 + 'a'), a, fla) == 1 ? 0 : 4);
	a[x][u] = 0;
	return judge == 0 ? 8 : judge;
}

void restore_fla(char fla[9][9])
{
	int i, j;
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			fla[i][j] = '\0';
}

void print(int a[9][9], char flag[9][9], char fla[9][9])
{
	int i, j;
	cout << "-+-a-b-c-+-d-e-f-+-g-h-i-" << endl;
	for (i = 0; i < 9; i++)
	{
		cout << i + 1 << "| ";
		for (j = 0; j < 9; j++)
		{
			if (flag[i][j] == '*')
				if (fla[i][j] == '#')
					cct_setcolor(15, 12);
				else if (fla[i][j] == '!')
					cct_setcolor(15, 9);
				else
					cct_setcolor(0, 9);
			else
				if (fla[i][j] == '#')
					cct_setcolor(15, 12);
				else if (fla[i][j] == '!')
					cct_setcolor(15, 14);
				else
					cct_setcolor(0, 14);
			cout << a[i][j];
			if (fla[i][j] == '!' || fla[i][j] == '#')
				cct_setcolor(15, 15);
			else
				cct_setcolor(0, 15);
			cout << ' ';
			cct_setcolor(0, 15);
			if (j % 3 == 2 && j != 8)
				cout << "| ";
		}
		if (i % 3 == 2 && i != 8)
		{
			cout << endl;
			cout << "-+-------+-------+-------";
		}
		cout << endl;
	}
}

int input(int a[9][9], char flag[9][9], int* steps, int* nx, char* ny, int* nu, char fla[9][9], int choice)
{
	int x, n, sum, t, x1, y1, o;
	char y, in[100] = { '\0' };
	if (choice == 2)
		cct_gotoxy(0, 30);
	cout << "请按行/列/值的方式输入(例如:5c6=第5行第c列为6)，输入bk表示回退一次：";
	cct_getxy(x1, y1);
	cin.getline(in, 100, '\n');
	if (choice == 2)
	{		
		cct_gotoxy(x1, y1);
		cout << setw(20) << setfill(' ') << ' ';
		cct_gotoxy(10, y1 + 1);
		cout << setw(20) << setfill(' ') << ' ' << endl;
		cct_gotoxy(0, y1 + 1);
		if (in[0] != '\n')
			cout << "本次输入：" << in << endl;
		else
			cout << "本次输入：" << endl;
		cct_getxy(x1, y1);
		cct_gotoxy(0, y1);
		cout << setw(40) << setfill(' ') << ' ';
		cct_gotoxy(0, y1);
	}
	if (strlen(in) == 2 && in[0] == 'b' && in[1] == 'k')
	{
		if (*steps == 0)
		{
			cout << "操作步骤已为空" << endl;
			return 0;
		}
		else
		{			
			cout << "本次输入正确" << endl;
			o = 9 + a[*nx - 1][*ny - 'a'];
			restore_fla(fla);
			a[*nx - 1][*ny - 'a'] = *nu;
			(*steps)--;			
			is_sudoku(a, fla);
			return o;
		}		
	}
	else
	{
		if (strlen(in) == 3)
		{
			x = in[0] - '0';
			y = in[1];
			n = in[2] - '0';
			if (x < 0 || x > 9)
			{
				cout << "行" << in[0] << "不是[0-9]，请重新输入" << endl;
				return 0;
			}
			else if (y < 'a' || y > 'i')
			{
				cout << "列" << in[1] << "不是[a-i]，请重新输入" << endl;
				return 0;
			}
			else if (n < 0 || n > 9)
			{
				cout << "值" << in[2] << "不是[0-9]，请重新输入" << endl;
				return 0;
			}
		}
		else
		{
			cout << "输入格式不正确，请重新输入" << endl;
			return 0;
		}
		switch (sum = check(x - 1, y, a, n, flag, fla))
		{
		case 9:
			cout << "该位置是题目原始值，不能改变，请重新输入" << endl;
			return 0;
		case 8:
		{
			cout << "本次输入正确" << endl;
			if (x == *nx && y == *ny)
				restore_fla(fla);
			*nu = a[x - 1][y - 'a'];
			a[x - 1][y - 'a'] = n;
			*nx = x;
			*ny = y;
			return 8;
		}
		default:
		{
			if (sum == 1 || sum == 2 || sum == 4)
				t = 1;
			else if (sum == 7)
				t = 3;
			else
				t = 2;
			if (choice == 1)
				cout << "检查后的数据有" << t << "处冲突，具体是：" << endl;
			else
				cout << "检查后的数据有" << t << "处冲突" << endl;
			*nu = a[x - 1][y - 'a'];
			a[x - 1][y - 'a'] = n;
			*nx = x;
			*ny = y;
			is_sudoku(a, fla);
			if (choice == 1)
				print(a, flag, fla);
			else
				draw(a, flag, fla);
			return sum;
			break;
		}
		}
	}
}