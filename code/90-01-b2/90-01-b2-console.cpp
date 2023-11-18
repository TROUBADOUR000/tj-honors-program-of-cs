/* 胡逸凡 信01 2153592 */
#include<iostream>
#include<iomanip>
#include<windows.h>
#include"90-01-b2.h"
#include "../include/cmd_console_tools.h"
using namespace std;

const char tabs[11][3] = { "┏","┓","┗","┛","━","┃","╋","┳","┻","┫","┣" };

void print_single(int a, int z)
{
	int x, y;
	cct_setcolor(a, 0 + z);
	cct_getxy(x, y);
	cout << tabs[0] << tabs[4] << tabs[1];
	cct_gotoxy(x, y + 1);
	cout << tabs[5] << a;
	if (a < 10)
		cout << ' ';
	cout << tabs[5];
	cct_gotoxy(x, y + 2);
	cout << tabs[2] << tabs[4] << tabs[3];
	cct_setcolor(0, 15);
}

void restore_pic(int row, int col, int arr[8][10], char mid[8][10], int b_or_w)
{
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			if (mid[i][j] == '*')
			{
				cct_gotoxy(4 + 8 * j, 3 + 4 * i);
				print_single(arr[i][j], b_or_w);
			}
}

void clear()
{
	int x, y, i;
	cct_getxy(x, y);
	cct_setcolor(15, 0);
	for (i = 0; i < 3; i++)
	{
		cct_gotoxy(x, y + i);
		if ((y + i) % 4 == 2)
			cout << "━━━";
		else
			cout << setfill(' ') << setw(6) << ' ';
	}
	cct_setcolor(0, 15);
}

void print_background(int col, int set_lines, int set_cols, int c)
{
	int i, j, r = 65;
	if (c == 0)
	{
		cout << ' ';
		for (i = 0; i < col; i++)
			cout << setfill(' ') << setw(6) << i;
	}
	else
	{
		cout << setfill(' ') << setw(6) << '0';
		for (i = 1; i < col; i++)
			cout << setfill(' ') << setw(8) << i;
	}
	cout << endl;
	if (c == 0)
		for (j = 3; j < set_lines - 3; j++)
		{
			if (j % 3 == 2)
				cout << char(r++) << ' ';
			else
				cout << "  ";
			cct_setcolor(15, 0);
			if (j == 3)
				cout << tabs[0];
			else if (j == set_lines - 4)
				cout << tabs[2];
			else
				cout << tabs[5];
			if (j == 3 || j == set_lines - 4)
				for (i = 0; i < (set_cols - 7) / 2; i++)
					cout << tabs[4];
			else
				for (i = 0; i < set_cols - 7; i++)
					cout << ' ';
			if (j == 3)
				cout << tabs[1];
			else if (j == set_lines - 4)
				cout << tabs[3];
			else
				cout << tabs[5];
			cct_setcolor(0, 15);
			Sleep(100);
			cout << endl;
		}
	else
		for (j = 3; j < set_lines - 3; j++)
		{
			if (j % 4 == 1)
				cout << char(r++) << ' ';
			else
				cout << "  ";
			cct_setcolor(15, 0);
			if (j == 3)
				cout << tabs[0];
			else if (j == set_lines - 4)
				cout << tabs[2];
			else if (j % 4 == 3)
				cout << tabs[10];
			else
				cout << tabs[5];
			if (j == 3 || j == set_lines - 4 || j % 4 == 3)
				for (i = 0; i < (set_cols - 7) / 2; i++)
				{
					if (i % 4 == 3)
					{
						if (j == 3)
							cout << tabs[7];
						else if (j == set_lines - 4)
							cout << tabs[8];
						else
							cout << tabs[6];
					}
					else
						cout << tabs[4];
				}
			else
				for (i = 0; i < (set_cols - 7) / 2; i++)
				{
					if (i % 4 == 3)
						cout << tabs[5];
					else
						cout << "  ";
				}
			if (j == 3)
				cout << tabs[1];
			else if (j == set_lines - 4)
				cout << tabs[3];
			else if (j % 4 == 3)
				cout << tabs[9];
			else
				cout << tabs[5];
			cct_setcolor(0, 15);
			Sleep(100);
			cout << endl;
		}
}

void fall(int row, int col, int arr[8][10], char mid[8][10], int z)
{
	int i, j, r, k;
	for (j = 0; j < col; j++)
	{
		r = row;
		while (r > 0)
		{
			for (i = 0; i < r; i++)
			{
				if (arr[i][j] != 0 && arr[i + 1][j] == 0 && i < row - 1)
				{
					swap(&arr[i][j], &arr[i + 1][j]);
					mid[i][j] = '*';
					mid[i + 1][j] = '0';
					if (arr[i + 1][j] != 0 && z > 6)
						for (k = 0; k < 4; k++)
						{
							cct_gotoxy(4 + 8 * j, 3 + 4 * i + k);
							clear();
							cct_gotoxy(4 + 8 * j, 3 + 4 * i + k + 1);
							print_single(arr[i + 1][j], 0);
							Sleep(10);
						}
				}
			}
			r--;
		}
	}
}

void twinkle(int* column, char* a, int arr[8][10], char mid[8][10])
{
	int i;
	mid[*a - 65][*column] = '0';
	for (i = 0; i < 10; i++)
	{
		cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
		print_single(arr[*a - 65][*column], 7);
		Sleep(20);
		cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
		print_single(arr[*a - 65][*column], 0);
		Sleep(20);
	}
}

void picture_5(int row, int col, int arr[8][10], int cct_lines, int set_cols)
{
	cct_setcursor(CURSOR_INVISIBLE);
	int i, j, t = 0;
	print_background(col, cct_lines, set_cols, 0);
	for (i = 4; i <= cct_lines - 5; i += 3)
	{
		t = (i - 3) / 4;
		for (j = 0; j < col; j++)
		{
			cct_gotoxy(4 + 6 * j, i - 1);
			print_single(arr[t][j], 0);
		}
	}
	cout << endl;
}

void picture_6(int row, int col, int arr[8][10], int cct_lines, int set_cols)
{
	cct_setcursor(CURSOR_INVISIBLE);
	int i, j, t;
	print_background(col, cct_lines, set_cols, 1);
	for (i = 4; i <= cct_lines - 5; i += 4)
	{
		t = i / 4 - 1;
		for (j = 0; j < col; j++)
		{
			cct_gotoxy(4 + 8 * j, i - 1);
			print_single(arr[t][j], 0);
			cct_setcolor(15, 0);
		}
	}
	cout << endl;
}

int judge1(int t)
{
	int X = 0, Y = 0, ret, maction, keycode1, keycode2, loop = 1, is_quit = 0;
	cct_enable_mouse();
	cct_setcursor(CURSOR_INVISIBLE);
	while (loop) {
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {
			switch (maction) {
			case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
				loop = 0;
				break;
			case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
				loop = 0;
				is_quit = 1;
				break;
			case MOUSE_NO_ACTION:
			default:
				break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
			case 13:	//回车键
				if (t != 1)
					loop = 0;
				break;
			case 67:
			case 99:
				if (t == 1)
					loop = 0;
				break;
			default:
				break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT）
	} //end of while(1)
	cct_disable_mouse();	//禁用鼠标
	cct_setcursor(CURSOR_VISIBLE_NORMAL);	//打开光标
	return is_quit;
}

int by_mouse_keyboard(int x, int y, int z, int set_lines, int arr[8][10], int row, int col, char mid[8][10], int* column, char* a)
{
	int ret, maction, keycode1, keycode2, loop = 1, X = 0, Y = 0, u, is_quit = 0, flag = 0;
	char v;
	cct_setcursor(CURSOR_INVISIBLE);
	cct_enable_mouse();
	while (loop)
	{
		u = *column;
		v = *a;
		if (z == 1)
			loop = 0;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT)
		{
			switch (maction)
			{
			case MOUSE_ONLY_MOVED:
				u = (X - 4) / 8;
				v = char(65 + (Y - 3) / 4);
				if (u != *column || v != *a)
					flag = 0;
				if (Y % 4 != 2 && Y < y - 1 && Y >= 3 && X % 8 != 3 && X > 4 && X < set_lines - 1)
				{
					if (u != *column || v != *a)
					{
						cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
						print_single(arr[*a - 65][*column], 0);
						cct_gotoxy(x, y);
						cout << setw(70) << ' ';
						cct_gotoxy(x, y);
						cout << "[当前鼠标] ";
						*column = (X - 4) / 8;
						*a = char(65 + (Y - 3) / 4);
						cout << *a << "行" << *column << "列";
						restore_pic(row, col, arr, mid, 0);
						cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
						print_single(arr[*a - 65][*column], 15);
						restore_mid(mid);
					}
				}
				else
				{
					cct_gotoxy(x, y);
					cout << setw(70) << ' ';
					cct_gotoxy(x, y);
					cout << "非法位置";
					restore_pic(row, col, arr, mid, 0);
					restore_mid(mid);
				}
				break;
			case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
				if (z > 7)
				{
					flag++;
					if (flag == 2)
						loop = 0;
					if (flag == 1)
					{
						recursion_judge(u, v, arr, mid, row, col);
						restore_pic(row, col, arr, mid, 7);
						cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
						print_single(arr[*a - 65][*column], 15);
						cct_gotoxy(x, y);
						cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
					}
				}
				if (z == 7)
					loop = 0;
				break;
			case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
				flag = 0;
				if (z >= 8)
				{
					loop = 0;
					is_quit = 1;
				}
				break;
			case MOUSE_NO_ACTION:
			default:
				flag = 0;
				break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT)
		{
			flag = 0;
			cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
			print_single(arr[*a - 65][*column], 0);
			switch (keycode1)
			{
			case 13:	//回车键
				if (z >= 7)
					loop = 0;
				break;
			case 81:
			case 113:
				if (z >= 8 || z == 1)
				{
					loop = 0;
					is_quit = 1;
				}
				break;
			case 224:
				switch (keycode2)
				{
				case KB_ARROW_UP:
					if (*a > 'A')
						(*a)--;
					else
						*a = char(row + 'A' - 1);
					break;
				case KB_ARROW_DOWN:
					if (*a<char(row + 'A' - 1))
						(*a)++;
					else
						*a = 'A';
					break;
				case KB_ARROW_LEFT:
					if (*column > 0)
						(*column)--;
					else
						*column = col - 1;
					break;
				case KB_ARROW_RIGHT:
					if (*column < col - 1)
						(*column)++;
					else
						*column = 0;
					break;
				}
				break;
			default:	//其他键不显示
				break;
			}//end of swicth(keycode1)
			cct_gotoxy(x, y);
			cout << setw(70) << ' ';
			cct_gotoxy(x, y);
			if (z != 10 || v != *a || u != *column)
				cout << "[当前按键] : " << *a << "行" << *column << "列";
			cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
			print_single(arr[*a - 65][*column], 15);
		}//end of else if(ret == CCT_KEYBOARD_EVENT）
	} //end of while(1)
	cct_disable_mouse();	//禁用鼠标
	cct_setcursor(CURSOR_VISIBLE_NORMAL);	//打开光标
	cct_gotoxy(x, y);
	cout << setw(70) << ' ';
	cct_gotoxy(x, y);
	if (z == 7)
		cout << "选中了" << *a << "行" << *column << "列";
	return is_quit;
}

void around_none(int x, int y, char mid[8][10], char v, int u)
{
	cct_gotoxy(x, y);
	cct_setcolor(0, 14);
	cout << "周围无相同值，";
	cct_setcolor(0, 7);
	cout << "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束";
	mid[v - 65][u] = '0';
}

int around_some(int x, int y, char mid[8][10], char v, int u, int arr[8][10], int row, int col, int* is_quit, int set_lines, int* column, char* a)
{
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			if (mid[i][j] == '*' && (i != v - 65 || j != u))
			{
				cct_gotoxy(4 + 8 * j, 3 + 4 * i);
				print_single(arr[i][j], 7);
			}
	cct_gotoxy(x, y);
	cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
	*is_quit = by_mouse_keyboard(x, y, 1, set_lines, arr, row, col, mid, column, a);
	if ((u != *column || v != *a) && *is_quit != 1)
	{
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)
				if (mid[i][j] == '*')
				{
					mid[i][j] = '0';
					cct_gotoxy(4 + 8 * j, 3 + 4 * i);
					print_single(arr[i][j], 0);
				}
		cct_gotoxy(4 + 8 * *column, 3 + 4 * (*a - 65));
		print_single(arr[*a - 65][*column], 15);
		cct_gotoxy(x, y);
		cout << "[当前键盘] : " << *a << "行" << *column << "列";
		return 0;
	}
	else
		return 1;
}

void single_step(int x, int y, int z, int set_lines, int arr[8][10], int row, int col, char mid[8][10], int* column, char* a, int dst)
{
	cct_setcursor(CURSOR_INVISIBLE);
	int u = 0, X = 0, Y = 0, re = 2, sum_grade = 0, grade = 0, sum = 0, is_quit = 0;
	char v = 'A';
	cout << "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束";
	while (is_quit != 1)
	{
		is_quit = by_mouse_keyboard(x, y, z, set_lines, arr, row, col, mid, column, a);
		if (is_quit != 1)
		{
			if (u != *column || v != *a)
			{
				cct_gotoxy(x, y);
				cout << setw(100) << ' ';
				cct_gotoxy(x, y);
				cct_gotoxy(4 + 8 * u, 3 + 4 * (v - 65));
				print_single(arr[v - 65][u], 0);
			}
			u = *column;
			v = *a;
			re = recursion_judge(u, v, arr, mid, row, col);
			if (re == 1)
				around_none(x, y, mid, v, u);
			else
				if (around_some(x, y, mid, v, u, arr, row, col, &is_quit, set_lines, column, a) == 1)
					break;
		}
	}
	if (is_quit != 1)
	{
		restore_arr(row, col, arr, mid, column, a);
		sum = num_mine(row, col, mid);
		arr[*a - 65][*column] += 1;
		pre_grade(sum, &sum_grade, arr[*a - 65][*column], dst);
		twinkle(column, a, arr, mid);
		cct_gotoxy(x, y);
		cout << "合成完成，回车键/单击左键下落0";
		judge1(0);
		fall(row, col, arr, mid, z);
		cct_gotoxy(x, y);
		cout << setw(70) << ' ';
		cct_gotoxy(x, y);
		cout << "下落0完成，回车键/单击左键填充新值";
		judge1(0);
		random_fill(row, col, arr, 8);
		judge1(0);
		cct_gotoxy(x, y);
		cct_setcolor(0, 14);
		cout << "本次合成结束，按C/单击左键继续新一次的合成";
		cct_setcolor(0, 7);
		judge1(1);
	}
}

void whole_steps(int x, int y, int set_lines, int arr[8][10], int row, int col, char mid[8][10], int* column, char* a, int dst)
{
	cct_setcursor(CURSOR_INVISIBLE);
	int u = 0, X = 0, Y = 0, re = 2, sum_grade = 0, grade = 0, sum, m = 0, z = 8, end = 0, is_quit = 0;
	char v = 'A';
	while (m < dst)
	{
		grade = 0;
		sum = 0;
		cct_gotoxy(x, y);
		cout << "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束";
		while (is_quit != 1)
		{
			restore_mid(mid);
			is_quit = by_mouse_keyboard(x, y, z, set_lines, arr, row, col, mid, column, a);
			if (is_quit != 1)
			{
				if (u != *column || v != *a)
				{
					cct_gotoxy(x, y);
					cout << setw(100) << ' ';
					cct_gotoxy(x, y);
					cct_gotoxy(4 + 8 * u, 3 + 4 * (v - 65));
					print_single(arr[v - 65][u], 0);
				}
				u = *column;
				v = *a;
				re = recursion_judge(u, v, arr, mid, row, col);
				if (re == 1)
					around_none(x, y, mid, v, u);
				else
					if (around_some(x, y, mid, v, u, arr, row, col, &is_quit, set_lines, column, a) == 1)
						break;
			}
		}
		if (is_quit != 1)
		{
			restore_arr(row, col, arr, mid, column, a);
			sum = num_mine(row, col, mid);
			arr[*a - 65][*column] += 1;
			pre_grade(sum, &sum_grade, arr[*a - 65][*column], dst);
			twinkle(column, a, arr, mid);
			fall(row, col, arr, mid, z);
			random_fill(row, col, arr, 8);
			end = is_end(arr, row, col);
			if (end == 1)
			{
				cct_gotoxy(x, y);
				cout << "游戏结束，无法合成" << endl;
				break;
			}
			m = max_arr(arr, col, row);
		}
		else
			break;
	}
	while (dst < 20 && is_quit != 1)
	{
		if (end == 1)
			break;
		cct_gotoxy(x, y);
		cct_setcolor(14, 12);
		cout << "已经合成到" << dst;
		cct_setcolor(0, 15);
		cout << ",按回车键/单击左键向更高目标进发..." << endl;
		is_quit = judge1(0);
		dst++;
		cct_gotoxy(0, 0);
		cout << "本次得分:" << grade << " 总得分:" << sum_grade << "合成目标:" << dst;
		while (m < dst)
		{
			grade = 0;
			sum = 0;
			cct_gotoxy(x, y);
			cout << "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束";
			while (1)
			{
				restore_mid(mid);
				by_mouse_keyboard(x, y, z, set_lines, arr, row, col, mid, column, a);
				if (u != *column || v != *a)
				{
					cct_gotoxy(x, y);
					cout << setw(100) << ' ';
					cct_gotoxy(x, y);
					cct_gotoxy(4 + 8 * u, 3 + 4 * (v - 65));
					print_single(arr[v - 65][u], 0);
				}
				u = *column;
				v = *a;
				re = recursion_judge(u, v, arr, mid, row, col);
				if (re == 1)
					around_none(x, y, mid, v, u);
				else
					if (around_some(x, y, mid, v, u, arr, row, col, &is_quit, set_lines, column, a) == 1)
						break;
			}
			restore_arr(row, col, arr, mid, column, a);
			sum = num_mine(row, col, mid);
			arr[*a - 65][*column] += 1;
			pre_grade(sum, &sum_grade, arr[*a - 65][*column], dst);
			twinkle(column, a, arr, mid);
			fall(row, col, arr, mid, z);
			random_fill(row, col, arr, 8);
			end = is_end(arr, row, col);
			if (end == 1)
			{
				cct_gotoxy(x, y);
				cout << "游戏结束，无法合成" << endl;
				break;
			}
			m = max_arr(arr, col, row);
		}
	}
}

void draw(int z)
{
	int row, col, dst, arr[8][10] = { 0 }, set_cols, set_lines, x, y, column = 0, ret = 0;
	char mid[8][10], a = 'A';
	input_init(&row, &col, &dst, mid, 1);
	cct_cls();
	array_element(row, col, arr);
	if (z == 5)
	{
		set_cols = 8 + 3 * row;
		set_lines = 7 + 6 * col;
	}
	else
	{
		set_cols = 7 + 4 * row;
		set_lines = 5 + 8 * col;
	}
	cct_setconsoleborder(set_lines, set_cols, 0, 0);
	cout << endl;
	if (z == 5)
		picture_5(row, col, arr, set_cols, set_lines);
	else
		picture_6(row, col, arr, set_cols, set_lines);
	cout << endl;
	cct_getxy(x, y);
	cct_gotoxy(0, 0);
	cct_setcolor(0, 15);
	cout << "屏幕当前设置为：" << set_cols << "行" << set_lines << "列" << endl;
	cct_gotoxy(x, y);
	if (z == 7)
	{
		cout << "箭头键/鼠标移动，回车键/单击左键选择并结束";
		by_mouse_keyboard(x, y, z, set_lines, arr, row, col, mid, &column, &a);
	}
	if (z == 8)
		single_step(x, y, z, set_lines, arr, row, col, mid, &column, &a, 5);
}

void game()
{
	int row, col, dst, arr[8][10] = { 0 }, set_cols, set_lines, x, y, column = 0;
	char mid[8][10], a = 'A';
	input_init(&row, &col, &dst, mid, 4);
	cct_cls();
	array_element(row, col, arr);
	set_cols = 7 + 4 * row;
	set_lines = 5 + 8 * col;
	cct_setconsoleborder(set_lines, set_cols, 0, 0);
	cout << endl;
	picture_6(row, col, arr, set_cols, set_lines);
	cout << endl;
	cct_getxy(x, y);
	cct_gotoxy(0, 0);
	cct_setcolor(0, 15);
	cout << "屏幕当前设置为：" << set_cols << "行" << set_lines << "列" << endl;
	cct_gotoxy(x, y);
	whole_steps(x, y, set_lines, arr, row, col, mid, &column, &a, dst);
}