/* 2153592 ÐÅ01 ºúÒÝ·² */
#include<iostream>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
using namespace std;

void single(int n, char flag, int x, int y, char fla, int choice)
{
	if (choice == 1)
	{
		if (flag == '*')
			if (fla == '#')
				cct_setcolor(15, 12);
			else if (fla == '!')
				cct_setcolor(15, 9);
			else
				cct_setcolor(0, 9);
		else
			if (fla == '#')
				cct_setcolor(15, 12);
			else if (fla == '!')
				cct_setcolor(15, 14);
			else
				cct_setcolor(0, 14);
	}
	else
		cct_setcolor(15, 12);
	cct_gotoxy(x, y);
	cout << "¨X¨T¨[" ;
	cct_gotoxy(x, y + 1);
	cout << "¨U" << n << " ¨U";
	cct_gotoxy(x, y + 2);
	cout << "¨^¨T¨a";
	cct_setcolor(0, 15);
}

void draw(int a[9][9], char flag[9][9], char fla[9][9])
{
	int i, j;
	cct_gotoxy(4, 1);
	cout << "    a     b     c     d     e     f     g     h     i" << endl;
	for (i = 0; i < 9; i++)
	{
		cct_gotoxy(3, 3 * (i + 1));
		cout << i + 1;
		for (j = 0; j < 9; j++)
			single(a[i][j], flag[i][j], 6 * j + 6, 3 * i + 2, fla[i][j], 1);
	}
}

void back()
{
	int x = 66, y = 25;
	cct_gotoxy(x, y);
	cout << "¨X¨T¨T¨[";
	cct_gotoxy(x, y + 1);
	cout << "¨Uback¨U";
	cct_gotoxy(x, y + 2);
	cout << "¨^¨T¨T¨a";
}

void temporary(int n, int x, int y)
{
	cct_setcolor(11, 13);
	cct_gotoxy(x, y);
	cout << "¨X¨T¨[";
	cct_gotoxy(x, y + 1);
	cout << "¨U" << n << " ¨U";
	cct_gotoxy(x, y + 2);
	cout << "¨^¨T¨a";
	cct_setcolor(0, 15);
}