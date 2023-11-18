/* 胡逸凡 信01 2153592 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include"90-01-b2.h"
#include "../include/cmd_console_tools.h"
using namespace std;

const int four[10] = { 1,1,1,2,2,2,3,3,3,4 }, five[20] = { 1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,5,5 }, six[20] = { 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,6 };

void array_element(int row, int col, int arr[8][10])
{
	srand((unsigned int)time(NULL));
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			arr[i][j] = rand() % 3 + 1;
}

void restore_mid(char mid[8][10])
{
	int i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 10; j++)
			mid[i][j] = '0';
}

void restore_arr(int row, int col, int arr[8][10], char mid[8][10], int* column, char* a)
{
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			if (mid[i][j] == '*' && (i != *a - 65 || j != *column))
			{
				cct_gotoxy(4 + 8 * j, 3 + 4 * i);
				clear();
				arr[i][j] = 0;
			}
}

int max_arr(int arr[8][10], int col, int row)
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

int num_mine(int row, int col, char mid[8][10])
{
	int i, j, sum = 0;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			if (mid[i][j] == '*')
				sum++;
	return sum;
}

void random_fill(int row, int col, int arr[8][10], int z)
{
	int i, j, t, m;
	m = max_arr(arr, col, row);
	srand((unsigned int)time(NULL));
	for (j = 0; j < col; j++)
		for (i = 0; i < row; i++)
		{
			if (arr[i][j] == 0)
			{
				if (m == 3)
					arr[i][j] = rand() % 3 + 1;
				else if (m == 4)
					arr[i][j] = four[rand() % 10];
				else if (m == 5)
					arr[i][j] = five[rand() % 20];
				else if (m == 6)
					arr[i][j] = six[rand() % 20];
				else
				{
					t = rand();
					if (t % 20 == 0)
						arr[i][j] = m;
					else if (t % 20 == 1)
						arr[i][j] = m - 1;
					else if (t % 20 == 3 || t % 20 == 4)
						arr[i][j] = m - 2;
					else
						arr[i][j] = t % (m - 3) + 1;
				}
				if (z > 7)
				{
					cct_gotoxy(4 + 8 * j, 3 + 4 * i);
					print_single(arr[i][j], 0);
					Sleep(10);
				}
			}
		}
}

void output_init(int row, int col, int arr[8][10])
{
	int i, j;
	cout << "当前数组：" << endl;
	cout << "  |";
	for (i = 0; i < col; i++)
		cout << "  " << i;
	cout << endl;
	cout << "--+" << setfill('-') << setw(3 * col + 1) << '-' << endl;
	for (i = 0; i < row; i++)
	{
		cout << char('A' + i) << " |";
		for (j = 0; j < col; j++)
			cout << setw(3) << setfill(' ') << arr[i][j];
		cout << endl;
	}
}

void output_ans(int row, int col, char mid[8][10])
{
	int i, j;
	cout << endl;
	cout << "查找结果数组：" << endl;
	cout << "  |";
	for (i = 0; i < col; i++)
		cout << "  " << i;
	cout << endl;
	cout << "--+" << setfill('-') << setw(3 * col + 1) << '-' << endl;
	for (i = 0; i < row; i++)
	{
		cout << char(65 + i) << " |";
		for (j = 0; j < col; j++)
			cout << setw(3) << setfill(' ') << mid[i][j];
		cout << endl;
	}
}

void output_now(int row, int col, char mid[8][10], int arr[8][10], int z)
{
	int i, j;
	cout << endl;
	if (z == 1)
		cout << "当前数组(不同色标识)：" << endl;
	else if (z == 2)
		cout << "相同值归并后的数组(不同色标识)：" << endl;
	else if (z == 3)
		cout << "除0后的数组(不同色标识)：" << endl;
	else
		cout << "新值填充后的数组(不同色标识)：" << endl;
	cout << "  |";
	for (i = 0; i < col; i++)
		cout << "  " << i;
	cout << endl;
	cout << "--+" << setfill('-') << setw(3 * col + 1) << '-' << endl;
	for (i = 0; i < row; i++)
	{
		cout << char(65 + i) << " |";
		for (j = 0; j < col; j++)
		{
			if (arr[i][j] < 10)
				cout << "  ";
			else
				cout << ' ';
			if (mid[i][j] == '*')
				cct_setcolor(14, 0);
			cout << arr[i][j];
			cct_setcolor(0, 7);
		}
		cout << endl;
	}
}

char merge(int row, int col, char mid[8][10], int arr[8][10], int num, char a, int dst, int z, int m, int* gra)
{
	int sum_grade = 0, grade = 0, x, y, i, j, sum = 0, re = 2;
	char choice;
	sum_grade = *gra;
	cout << endl;
	cout << "请确认是否把相邻的相同值合并到C1中(Y/N/Q)：";
	cct_getxy(x, y);
	while (1)
	{
		choice = _getch();
		if (choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n' || choice == 'q' || choice == 'Q')
		{
			cout << choice << endl;
			break;
		}
	}
	if (choice == 'Y' || choice == 'y')
	{
		cout << endl;
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)
				if (mid[i][j] == '*')
				{
					sum++;
					if (i == int(a - 65) && j == num)
						arr[i][j] += 1;
					else
						arr[i][j] = 0;
				}
		output_now(row, col, mid, arr, 2);
		grade = (arr[int(a - 65)][num] - 1) * sum * 3;
		sum_grade += grade;
		cout << endl;
		cout << "本次得分：" << grade << " 总得分：" << sum_grade << " 合成目标：" << dst << endl;
		*gra = sum_grade;
		cout << endl;
		cout << "按回车键进行数组下落除0操作...";
		mid[int(a - 65)][num] = '0';
		while (_getch() != '\r');
		cout << endl;
		fall(row, col, arr, mid, 4);
		output_now(row, col, mid, arr, 3);
		cout << endl;
		cout << "按回车键进行新值填充...";
		while (_getch() != '\r');
		cout << endl;
		random_fill(row, col, arr, 4);
		output_now(row, col, mid, arr, 4);
	}
	else if (z != 3 && (choice == 'N' || choice == 'n'))
	{
		cout << endl;
		output_init(row, col, arr);
		restore_mid(mid);
		cout << endl;
		while (re != 0)
		{
			input_tmp(&num, &a, row, col, arr, mid, re);
			re = recursion_judge(num, a, arr, mid, row, col);
		}
		output_ans(row, col, mid);
		output_now(row, col, mid, arr, 1);
		merge(row, col, mid, arr, num, a, dst, z, m, gra);
	}
	else
		return 'q';
	return '0';
}

void output_move(int row, int col, char mid[8][10], int arr[8][10], int dst, int z)
{
	int m = 3, re = 2, num, gra = 0, end = 0;
	char a, ju = '0';
	while (m < dst)
	{
		output_init(row, col, arr);
		cout << endl;
		re = 2;
		restore_mid(mid);
		while (re != 0)
		{
			input_tmp(&num, &a, row, col, arr, mid, re);
			re = recursion_judge(num, a, arr, mid, row, col);
		}
		output_ans(row, col, mid);
		output_now(row, col, mid, arr, 1);
		ju = merge(row, col, mid, arr, num, a, dst, z, m, &gra);
		if (ju == 'q')
			break;
		end = is_end(arr, row, col);
		if (end == 0)
			cout << "本次合成结束，按回车键继续新一次的合成..." << endl;
		else
		{
			cout << "游戏结束，无法合成" << endl;
			break;
		}
		while (_getch() != '\r');
		m = max_arr(arr, col, row);
	}
	while (dst < 20 && ju != 'q')
	{
		if (end == 1)
			break;
		cct_setcolor(14, 12);
		cout << "已经合成到" << dst << endl;
		cct_setcolor(0, 15);
		cout << ",按回车键向更高目标进发..." << endl;
		while (_getch() != '\r');
		cout << endl;
		dst++;
		while (m < dst)
		{
			output_init(row, col, arr);
			cout << endl;
			re = 2;
			restore_mid(mid);
			while (re != 0)
			{
				input_tmp(&num, &a, row, col, arr, mid, re);
				re = recursion_judge(num, a, arr, mid, row, col);
			}
			output_ans(row, col, mid);
			output_now(row, col, mid, arr, 1);
			ju = merge(row, col, mid, arr, num, a, dst, z, m, &gra);
			if (ju == 'q')
				break;
			end = is_end(arr, row, col);
			if (end == 0)
				cout << "本次合成结束，按回车键继续新一次的合成..." << endl;
			else
			{
				cout << "游戏结束，无法合成" << endl;
				break;
			}
			while (_getch() != '\r');
			cout << endl;
			m = max_arr(arr, col, row);
		}
	}
	if (dst == 20)
		cout << "游戏结束,恭喜过关" << endl;
}

void fundation(int z)
{
	int row, col, dst, arr[8][10] = { 0 }, num, re = 2, grade = 0;
	char a, mid[8][10];
	input_init(&row, &col, &dst, mid, z);
	array_element(row, col, arr);
	cout << endl;
	output_init(row, col, arr);
	cout << endl;
	while (re != 0)
	{
		input_tmp(&num, &a, row, col, arr, mid, re);
		if (z == 1)
			re = judge(num, a, arr, mid, row, col);
		else
			re = recursion_judge(num, a, arr, mid, row, col);
	}
	output_ans(row, col, mid);
	output_now(row, col, mid, arr, 1);
	if (z > 2)
		merge(row, col, mid, arr, num, a, dst, z, 3, &grade);
}

void full_edition()
{
	int row, col, dst, arr[8][10] = { 0 };
	char mid[8][10];
	input_init(&row, &col, &dst, mid, 4);
	array_element(row, col, arr);
	cout << endl;
	output_move(row, col, mid, arr, dst, 4);
}