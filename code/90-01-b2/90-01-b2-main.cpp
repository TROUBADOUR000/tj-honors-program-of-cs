/* 胡逸凡 信01 2153592 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include"90-01-b2.h"
#include "../include/cmd_console_tools.h"
#include "../include/public_use.h"
#define NUM 50
using namespace std;

int main()
{
	char a[NUM];
	int x, y;
	while (1)
	{
		switch (menu("1.命令行找出可合成项并标识（非递归）\n"
			"2.命令行找出可合成项并标识（递归）\n"
			"3.命令行完成一次合成（分步骤显示）\n"
			"4.命令行完整版（分步骤显示）\n"
			"5.伪图形界面显示初始数组（无分隔线）\n"
			"6.伪图形界面显示初始数组（有分隔线）\n"
			"7.伪图形界面下用箭头键 / 鼠标选择当前色块\n"
			"8.伪图形界面完成一次合成（分步骤）\n"
			"9.伪图形界面完整版(支持鼠标)\n"
			"0.退出\n","1234567890"))
		{
		case '0':
		{
			cct_gotoxy(0, 24);
			return 0;
		}
		case'1':
			fundation(1);
			break;
		case'2':
			fundation(2);
			break;
		case'3':
			fundation(3);
			break;
		case'4':
			full_edition();
			break;
		case'5':
			draw(5);
			break;
		case'6':
			draw(6);
			break;
		case'7':
			draw(7);
			break;
		case'8':
			draw(8);
			break;
		case'9':
			game();
			break;
		}
		cout << endl;
		cout << "本小题结束，请输入End继续...";
		cct_getxy(x, y);
		while (1)
		{
			cin >> a;
			if ((a[0] == 'e' || a[0] == 'E') && (a[1] == 'n' || a[1] == 'N') && (a[2] == 'd' || a[2] == 'D') && strlen(a) == 3)
				break;
			else
			{
				cin.clear();
				cin.ignore();
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