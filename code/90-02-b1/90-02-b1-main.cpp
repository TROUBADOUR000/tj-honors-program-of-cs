/* 2153592 信01 胡逸凡 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include<fstream>
#include "../include/cmd_console_tools.h"
#include "../include/public_use.h"
#include "90-02-b1.h"
#define NUM 50
using namespace std;

int main()
{
	char a[NUM];
	int x, y;
	while (1)
	{
		switch (menu("1.字符界面游戏(带退回功能)\n"
			"2.图形界面游戏(带退回功能)\n"
			"3.图形界面自动解(显示过程并带延时)\n"
			"4.图形界面箭头键控制游戏(带回退功能)\n"
			"5.图形界面鼠标控制游戏(带回退功能)\n"
			"0.退出\n","123450"))
		{
		case '0':
		{
			cct_gotoxy(0, 24);
			return 0;
		}
		case'1':
			character_pattern(1);
			break;
		case'2':
			character_pattern(2);
			break;
		case'3':
			auto_solution();
			break;
		case'4':
			control(2);
			break;
		case'5':
			control(1);
			break;
		}
		cout << endl;
		cout << "本小题结束，请输入End继续...";
		cct_getxy(x, y);
		while (1)
		{
			cin.get(a, NUM, '\n');
			cin.clear();
			cin.ignore();
			if ((a[0] == 'e' || a[0] == 'E') && (a[1] == 'n' || a[1] == 'N') && (a[2] == 'd' || a[2] == 'D') && strlen(a) == 3)
			    break;
			else
			{				
				cout << "输入错误，请重新输入";
				cct_gotoxy(x, y);
				cout << setfill(' ') << setw(NUM) << ' ';
				cct_gotoxy(x, y);
			}
		}
		cct_cls();
	}
	return 0;
}
