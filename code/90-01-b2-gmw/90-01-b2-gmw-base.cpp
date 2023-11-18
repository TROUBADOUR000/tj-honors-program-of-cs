/* ºúÒÝ·² ÐÅ01 2153592 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include"90-01-b2-gmw.h"
#include "../include/cmd_console_tools.h"
using namespace std;

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

void swap(int* a, int* b)
{
	int t;
	t = *a;
	*a = *b;
	*b = t;
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