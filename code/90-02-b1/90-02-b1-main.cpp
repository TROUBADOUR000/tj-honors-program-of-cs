/* 2153592 ��01 ���ݷ� */
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
		switch (menu("1.�ַ�������Ϸ(���˻ع���)\n"
			"2.ͼ�ν�����Ϸ(���˻ع���)\n"
			"3.ͼ�ν����Զ���(��ʾ���̲�����ʱ)\n"
			"4.ͼ�ν����ͷ��������Ϸ(�����˹���)\n"
			"5.ͼ�ν�����������Ϸ(�����˹���)\n"
			"0.�˳�\n","123450"))
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
		cout << "��С�������������End����...";
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
				cout << "�����������������";
				cct_gotoxy(x, y);
				cout << setfill(' ') << setw(NUM) << ' ';
				cct_gotoxy(x, y);
			}
		}
		cct_cls();
	}
	return 0;
}
