/* ���ݷ� ��01 2153592 */
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
		switch (menu("1.�������ҳ��ɺϳ����ʶ���ǵݹ飩\n"
			"2.�������ҳ��ɺϳ����ʶ���ݹ飩\n"
			"3.���������һ�κϳɣ��ֲ�����ʾ��\n"
			"4.�����������棨�ֲ�����ʾ��\n"
			"5.αͼ�ν�����ʾ��ʼ���飨�޷ָ��ߣ�\n"
			"6.αͼ�ν�����ʾ��ʼ���飨�зָ��ߣ�\n"
			"7.αͼ�ν������ü�ͷ�� / ���ѡ��ǰɫ��\n"
			"8.αͼ�ν������һ�κϳɣ��ֲ��裩\n"
			"9.αͼ�ν���������(֧�����)\n"
			"0.�˳�\n","1234567890"))
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
		cout << "��С�������������End����...";
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