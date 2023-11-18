/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>  
#include "90-01-b2-gmw-cft.h"
#include "../include/cmd_console_tools.h"
#define NUM 20
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

void input_init(int* row, int* col, int* dst, char mid[8][10])
{
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
	cin.ignore(1024, '\n');
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

void restore_arr(CONSOLE_GRAPHICS_INFO* mto_10, int arr[8][10], char mid[8][10], int MRow, int MCol, const BLOCK_DISPLAY_INFO bdi_normal[])
{
	int i, j;
	for (i = 0; i < mto_10->row_num; i++)
		for (j = 0; j < mto_10->col_num; j++)
			if (mid[i][j] == '*' && (i != MRow || j != MCol))
			{
				arr[i][j] = 0;
				gmw_draw_block(mto_10, i, j, 0, bdi_normal);
			}
}

void random_fill(CONSOLE_GRAPHICS_INFO* mto_10, int arr[8][10], const BLOCK_DISPLAY_INFO bdi_normal[])
{
	const int four[10] = { 1,1,1,2,2,2,3,3,3,4 }, five[20] = { 1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,5,5 },
		six[20] = { 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,6 };
	int i, j, t, m;
	m = max_arr(arr, mto_10->col_num, mto_10->row_num);
	srand((unsigned int)time(NULL));
	for (j = 0; j < mto_10->col_num; j++)
		for (i = 0; i < mto_10->row_num; i++)
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
				gmw_draw_block(mto_10, i, j, arr[i][j], bdi_normal);
			}
		}
}

int keyboard_mouse(CONSOLE_GRAPHICS_INFO* mto_10, int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2,
	int arr[8][10], int choice, const BLOCK_DISPLAY_INFO bdi_normal[], const BLOCK_DISPLAY_INFO bdi_selected[])
{
	cct_enable_mouse();
	cct_setcursor(CURSOR_INVISIBLE);
	int ret, loop = 1, is_quit = 0, old_row, old_col;
	gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
	while (loop)
	{
		old_col = MCol;
		old_row = MRow;
		ret = gmw_read_keyboard_and_mouse(mto_10, MAction, MRow, MCol, KeyCode1, KeyCode2);
		if (ret == CCT_KEYBOARD_EVENT)
		{
			switch (KeyCode1)
			{
			case '\r':
				loop = 0;
				break;
			case 'q':
			case 'Q':
				loop = 0;
				is_quit = 1;
				break;
			case 224:
				switch (KeyCode2)
				{
				case KB_ARROW_UP:
				{
					if (MRow > 0)
						MRow--;
					else
						MRow = mto_10->row_num - 1;
					break;
				}
				case KB_ARROW_DOWN:
				{
					if (MRow < mto_10->row_num - 1)
						MRow++;
					else
						MRow = 0;
					break;
				}
				case KB_ARROW_LEFT:
				{
					if (MCol > 0)
						MCol--;
					else
						MCol = mto_10->col_num - 1;
					break;
				}
				case KB_ARROW_RIGHT:
				{
					if (MCol < mto_10->col_num - 1)
						MCol++;
					else
						MCol = 0;
					break;
				}
				}
			}
			if (old_col != MCol || old_row != MRow)
			{
				gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
				gmw_draw_block(mto_10, old_row, old_col, arr[old_row][old_col], bdi_normal);
				if (choice)
					loop = 0;
			}
		}
		else
		{
			switch (MAction)
			{
			case  MOUSE_LEFT_BUTTON_CLICK:
				loop = 0;
				break;
			case MOUSE_RIGHT_BUTTON_CLICK:
				is_quit = 1;
				loop = 0;
				break;
			case MOUSE_ONLY_MOVED:
				if (old_col != -1 && old_row != -1 && (old_col != MCol || old_row != MRow))
				{
					gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
					gmw_draw_block(mto_10, old_row, old_col, arr[old_row][old_col], bdi_normal);
					if (choice)
						loop = 0;
				}
				break;
			default:
				break;
			}
		}
	}
	cct_disable_mouse();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	return is_quit;
}

int around_some(CONSOLE_GRAPHICS_INFO* mto_10, char mid[8][10], char old_row, int old_col, int arr[8][10], int& is_quit,
	const BLOCK_DISPLAY_INFO bdi_normal[], const BLOCK_DISPLAY_INFO bdi_related[], const BLOCK_DISPLAY_INFO bdi_selected[],
	int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2)
{
	int i, j;
	for (i = 0; i < mto_10->row_num; i++)
		for (j = 0; j < mto_10->col_num; j++)
			if (mid[i][j] == '*' && (i != old_row - 65 || j != old_col))
				gmw_draw_block(mto_10, i, j, arr[i][j], bdi_related);
	gmw_status_line(mto_10, LOWER_STATUS_LINE, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束");
	is_quit = keyboard_mouse(mto_10, MAction, MRow, MCol, KeyCode1, KeyCode2, arr, 1, bdi_normal, bdi_selected);
	if ((old_col != MCol || old_row - 65 != MRow) && !is_quit)
	{
		for (i = 0; i < mto_10->row_num; i++)
			for (j = 0; j < mto_10->col_num; j++)
				if (mid[i][j] == '*')
				{
					mid[i][j] = '0';
					gmw_draw_block(mto_10, i, j, arr[i][j], bdi_normal);
				}
		gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
		return 0;
	}
	else
		return 1;
}

void game_without_separator(CONSOLE_GRAPHICS_INFO* mto_10, int dst, char mid[8][10], int arr[8][10],
	const BLOCK_DISPLAY_INFO bdi_normal[], const BLOCK_DISPLAY_INFO bdi_related[], const BLOCK_DISPLAY_INFO bdi_selected[])
{
	int old_col = 0, X = 0, Y = 0, re = 2, sum_grade = 0, sum, m = 0, end = 0, is_quit = 0, pre_grade = 0;
	char old_row = 'A', temp[256];
	int MAction, MRow = 0, MCol = 0, KeyCode1, KeyCode2, i, j, r;
	while (m < dst)
	{
		sum = 0;
		gmw_status_line(mto_10, LOWER_STATUS_LINE, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束");
		while (!is_quit)
		{
			restore_mid(mid);
			is_quit = keyboard_mouse(mto_10, MAction, MRow, MCol, KeyCode1, KeyCode2, arr, 0, bdi_normal, bdi_selected);
			if (!is_quit)
			{
				if (old_col != MCol || old_row != char(MRow + 'A'))
				{
					cct_gotoxy(0, mto_10->extern_up_lines + mto_10->CFI.bhigh + mto_10->SLI.is_top_status_line + mto_10->draw_frame_with_col_no);
					cout << setw(mto_10->cols - 1) << setfill(' ') << ' ';
					gmw_draw_block(mto_10, old_row - 65, old_col, arr[old_row - 65][old_col], bdi_normal);
				}
				old_col = MCol;
				old_row = char(MRow + 'A');
				re = recursion_judge(old_col, old_row, arr, mid, mto_10->row_num, mto_10->col_num);
				if (re == 1)
				{
					gmw_status_line(mto_10, LOWER_STATUS_LINE, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束", "周围无相同值，");
					mid[old_row - 65][old_col] = '0';
				}
				else
					if (around_some(mto_10, mid, old_row, old_col, arr, is_quit, bdi_normal, bdi_related, bdi_selected, MAction, MRow, MCol, KeyCode1, KeyCode2))
						break;
			}
		}
		if (!is_quit)
		{
			restore_arr(mto_10, arr, mid, MRow, MCol, bdi_normal);
			sum = num_mine(mto_10->row_num, mto_10->col_num, mid);
			arr[MRow][MCol] += 1;
			pre_grade = (arr[MRow][MCol] - 1) * sum * 3;
			sum_grade += pre_grade;
			sprintf(temp, "合成目标:%d 本次得分:%d 总得分:%d", dst, pre_grade, sum_grade);
			gmw_status_line(mto_10, TOP_STATUS_LINE, temp);
			for (int k = 0; k < 5; k++)
			{
				gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
				Sleep(50);
				gmw_draw_block(mto_10, MRow, MCol, 0, bdi_normal);
				Sleep(50);
			}
			gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
			for (j = 0; j < mto_10->col_num; j++)
			{
				r = mto_10->row_num;
				while (r > 0)
				{
					for (i = r - 1; i > 0; i--)
						if (arr[i][j] == 0)
							for (int k = i - 1; k >= 0; k--)
								if (arr[k][j] != 0)
								{
									gmw_move_block(mto_10, k, j, arr[k][j], 0, bdi_normal, UP_TO_DOWN, i - k);
									swap(&arr[i][j], &arr[k][j]);
									mid[i][j] = '*';
									mid[k][j] = '0';
									break;
								}
					r--;
				}
			}
			random_fill(mto_10, arr, bdi_normal);
			end = is_end(arr, mto_10->row_num, mto_10->col_num);
			if (end == 1)
			{
				gmw_status_line(mto_10, LOWER_STATUS_LINE, "游戏结束，无法合成");
				break;
			}
			m = max_arr(arr, mto_10->col_num, mto_10->row_num);
		}
		else
			break;
	}
	while (dst < 20 && is_quit != 1)
	{
		if (end == 1)
			break;
		sprintf(temp, "已经合成到%d", dst);
		gmw_status_line(mto_10, LOWER_STATUS_LINE, ",按回车键向更高目标进发...", temp);
		while (_getch() != '\r');
		dst++;
		sprintf(temp, "合成目标:%d 总得分:%d", dst, sum_grade);
		gmw_status_line(mto_10, TOP_STATUS_LINE, temp);
		while (m < dst)
		{
			sum = 0;
			gmw_status_line(mto_10, LOWER_STATUS_LINE, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束");
			while (!is_quit)
			{
				restore_mid(mid);
				is_quit = keyboard_mouse(mto_10, MAction, MRow, MCol, KeyCode1, KeyCode2, arr, 0, bdi_normal, bdi_selected);
				if (!is_quit)
				{
					if (old_col != MCol || old_row != char(MRow + 'A'))
					{
						cct_gotoxy(0, mto_10->extern_up_lines + mto_10->CFI.bhigh + mto_10->SLI.is_top_status_line + mto_10->draw_frame_with_col_no);
						cout << setw(mto_10->cols - 1) << setfill(' ') << ' ';
						gmw_draw_block(mto_10, old_row - 65, old_col, arr[old_row - 65][old_col], bdi_normal);
					}
					old_col = MCol;
					old_row = char(MRow + 'A');
					re = recursion_judge(old_col, old_row, arr, mid, mto_10->row_num, mto_10->col_num);
					if (re == 1)
					{
						gmw_status_line(mto_10, LOWER_STATUS_LINE, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束", "周围无相同值，");
						mid[old_row - 65][old_col] = '0';
					}
					else
						if (around_some(mto_10, mid, old_row, old_col, arr, is_quit, bdi_normal, bdi_related, bdi_selected, MAction, MRow, MCol, KeyCode1, KeyCode2))
							break;
				}
			}
			if (!is_quit)
			{
				restore_arr(mto_10, arr, mid, MRow, MCol, bdi_normal);
				sum = num_mine(mto_10->row_num, mto_10->col_num, mid);
				arr[MRow][MCol] += 1;
				pre_grade = (arr[MRow][MCol] - 1) * sum * 3;
				sum_grade += pre_grade;
				sprintf(temp, "合成目标:%d 本次得分:%d 总得分:%d", dst, pre_grade, sum_grade);
				gmw_status_line(mto_10, TOP_STATUS_LINE, temp);
				for (int k = 0; k < 5; k++)
				{
					gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
					Sleep(50);
					gmw_draw_block(mto_10, MRow, MCol, 0, bdi_normal);
					Sleep(50);
				}
				gmw_draw_block(mto_10, MRow, MCol, arr[MRow][MCol], bdi_selected);
				for (j = 0; j < mto_10->col_num; j++)
				{
					r = mto_10->row_num;
					while (r > 0)
					{
						for (i = r - 1; i > 0; i--)
							if (arr[i][j] == 0)
								for (int k = i - 1; k >= 0; k--)
									if (arr[k][j] != 0)
									{
										gmw_move_block(mto_10, k, j, arr[k][j], 0, bdi_normal, UP_TO_DOWN, i - k);
										swap(&arr[i][j], &arr[k][j]);
										mid[i][j] = '*';
										mid[k][j] = '0';
										break;
									}
						r--;
					}
				}
				random_fill(mto_10, arr, bdi_normal);
				end = is_end(arr, mto_10->row_num, mto_10->col_num);
				if (end == 1)
				{
					gmw_status_line(mto_10, LOWER_STATUS_LINE, "游戏结束，无法合成");
					break;
				}
				m = max_arr(arr, mto_10->col_num, mto_10->row_num);
			}
			else
				break;
		}
	}
}


void cft_file_read(CONSOLE_GRAPHICS_INFO& mto_10, cft& fcfg, int& row, int& col, int& dst, int bdi_nor_bg[10],
	int bdi_nor_fg[10], int& bdi_rel_bg, int& bdi_rel_fg, int& bdi_sel_bg, int& bdi_sel_fg, int& remove_bg, int& remove_fg)
{
	const int group_num = 4;
	const string group_name[group_num] = { "区域设置","边框设置","色块设置","状态栏设置" };
	fcfg.set_comment_switch(CommentSwitch::on);
	int i;
	string tmp = "";

	row = 5;
	col = 5;

	//[区域设置]
	//空行列数
	int init_row = 0, init_col = 0, down_row = 0, right_col = 0;
	if (fcfg.item_get_value(group_name[0].c_str(), "起始行号", i) > 0 && i >= 0 && i <= 15)
		init_row = i;
	if (fcfg.item_get_value(group_name[0].c_str(), "起始列号", i) > 0 && i >= 0 && i <= 15)
		init_col = i;
	if (fcfg.item_get_value(group_name[0].c_str(), "下方空行", i) > 0 && i >= 0 && i <= 15)
		down_row = i;
	if (fcfg.item_get_value(group_name[0].c_str(), "右侧空列", i) > 0 && i >= 0 && i <= 15)
		right_col = i;
	gmw_set_ext_rowcol(&mto_10, init_row, init_col, down_row, right_col);

	if (fcfg.item_get_value(group_name[0].c_str(), "行数", i) > 0 && i >= 5 && i <= 8)
		row = i;
	if (fcfg.item_get_value(group_name[0].c_str(), "列数", i) > 0 && i >= 5 && i <= 10)
		col = i;
	gmw_set_rowcol(&mto_10, row, col);

	//颜色
	int fgcolor = COLOR_WHITE, bgcolor = COLOR_BLACK;
	if (fcfg.item_get_value(group_name[0].c_str(), "背景色", i) > 0 && i >= 0 && i <= 15)
		bgcolor = i;
	if (fcfg.item_get_value(group_name[0].c_str(), "前景色", i) > 0 && i >= 0 && i <= 15)
		fgcolor = i;
	gmw_set_color(&mto_10, bgcolor, fgcolor);

	//字体字号
	int type_width = 8, type_height = 16;
	string typeface = "点阵字体";
	if (fcfg.item_get_value(group_name[0].c_str(), "字体", tmp) > 0 && (tmp == "点阵字体" || tmp == "新宋体"))
		typeface = tmp;
	if (fcfg.item_get_value(group_name[0].c_str(), "字号", i) > 0) {
		if (typeface == "点阵字体") {
			if (i == 6 || i == 8 || i == 10) {
				type_width = i;
				type_height = i * 2;
			}
		}
		else {
			if (i >= 8 && i <= 24)
			{
				type_width = i;
				type_height = i;
			}
			else
			{
				type_width = 12;
				type_height = 12;
			}
			if (type_width % 2)
				type_height = ++type_width;
		}
	}

	gmw_set_font(&mto_10, typeface.c_str(), type_height, type_width);

	//画图与移动的延时
	int delay_draw = 0, delay_move = 10;
	if (fcfg.item_get_value(group_name[0].c_str(), "画边框延时", i) > 0 && i >= 0 && i <= 100)
		delay_draw = i;
	gmw_set_delay(&mto_10, DELAY_OF_DRAW_FRAME, delay_draw);
	if (fcfg.item_get_value(group_name[0].c_str(), "色块移动延时", i) > 0 && i >= 3 && i <= 100)
		delay_move = i;
	gmw_set_delay(&mto_10, DELAY_OF_BLOCK_MOVED, delay_move);

	//合成目标
	dst = 5;
	if (fcfg.item_get_value(group_name[0].c_str(), "合成目标", i) > 0 && i >= 5 && i <= 99)
		dst = i;

	//[边框设置]
	//边线种类
	fgcolor = COLOR_BLACK;
	bgcolor = COLOR_HWHITE;
	char frame_type[5], choice = 'y';
	bool separator = 1, rank_no = 1;
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-左上角", frame_type) > 0)
		strcpy(mto_10.CFI.top_left, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-左下角", frame_type) > 0)
		strcpy(mto_10.CFI.lower_left, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-右上角", frame_type) > 0)
		strcpy(mto_10.CFI.top_right, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-右下角", frame_type) > 0)
		strcpy(mto_10.CFI.lower_right, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-横线", frame_type) > 0)
		strcpy(mto_10.CFI.h_normal, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-竖线", frame_type) > 0)
		strcpy(mto_10.CFI.v_normal, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-上分隔线", frame_type) > 0)
		strcpy(mto_10.CFI.h_top_separator, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-下分隔线", frame_type) > 0)
		strcpy(mto_10.CFI.h_lower_separator, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-左分隔线", frame_type) > 0)
		strcpy(mto_10.CFI.v_left_separator, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-右分隔线", frame_type) > 0)
		strcpy(mto_10.CFI.v_right_separator, frame_type);
	if (fcfg.item_get_value(group_name[1].c_str(), "边框线-中分隔线", frame_type) > 0)
		strcpy(mto_10.CFI.mid_separator, frame_type);

	//分隔线
	if (fcfg.item_get_value(group_name[1].c_str(), "需要分隔线", choice) > 0)
		if (choice == 'y' || choice == 'Y')
			separator = true;
		else if (choice == 'n' || choice == 'N')
			separator = false;
	

	//行标列标
	if (fcfg.item_get_value(group_name[1].c_str(), "行号列标显示", choice) > 0)
		if (choice == 'y' || choice == 'Y')
			rank_no = true;
		else if (choice == 'n' || choice == 'N')
			rank_no = false;
	gmw_set_rowno_switch(&mto_10, rank_no);

	//颜色
	fgcolor = COLOR_BLACK;
	bgcolor = COLOR_HWHITE;
	if (fcfg.item_get_value(group_name[1].c_str(), "背景色", i) > 0 && i >= 0 && i <= 15)
		bgcolor = i;
	if (fcfg.item_get_value(group_name[1].c_str(), "前景色", i) > 0 && i >= 0 && i <= 15)
		fgcolor = i;
	gmw_set_frame_color(&mto_10, bgcolor, fgcolor);

	//[色块设置]
	char block_type[5];

	//色块大小
	mto_10.CFI.block_high = 3;
	mto_10.CFI.block_width = 6;
	if (fcfg.item_get_value(group_name[2].c_str(), "宽度", i) > 0)
		if (i >= 6 || i <= 16)
			mto_10.CFI.block_width = i;
	if (fcfg.item_get_value(group_name[2].c_str(), "高度", i) > 0)
		if (i >= 3 || i <= 6)
			mto_10.CFI.block_high = i;

	gmw_set_frame_style(&mto_10, mto_10.CFI.block_width, mto_10.CFI.block_high, separator);

	//色块颜色
	int a;
	char name[15];
	for (int i = 0; i < 10; i++) {
		fgcolor = COLOR_BLACK;
		bgcolor = COLOR_HWHITE;
		sprintf(name, "色块%d-前景色", i + 1);
		if (fcfg.item_get_value(group_name[2].c_str(), name, a) > 0 && a >= 0 && a <= 15)
			bdi_nor_fg[i] = a;
		sprintf(name, "色块%d-背景色", i + 1);
		if (fcfg.item_get_value(group_name[2].c_str(), name, a) > 0 && a >= 0 && a <= 15)
			bdi_nor_bg[i] = a;
	}

	//色块边框
	//缺省
	strcpy(mto_10.CBI.top_left, "┏");
	strcpy(mto_10.CBI.lower_left, "┗");
	strcpy(mto_10.CBI.top_right, "┓");
	strcpy(mto_10.CBI.lower_right, "┛");
	strcpy(mto_10.CBI.h_normal, "━");
	strcpy(mto_10.CBI.v_normal, "┃");
	if (fcfg.item_get_value(group_name[2].c_str(), "边框线-左上角", block_type) > 0)
		strcpy(mto_10.CBI.top_left, block_type);
	if (fcfg.item_get_value(group_name[2].c_str(), "边框线-左下角", block_type) > 0)
		strcpy(mto_10.CBI.lower_left, block_type);
	if (fcfg.item_get_value(group_name[2].c_str(), "边框线-右上角", block_type) > 0)
		strcpy(mto_10.CBI.top_right, block_type);
	if (fcfg.item_get_value(group_name[2].c_str(), "边框线-右下角", block_type) > 0)
		strcpy(mto_10.CBI.lower_right, block_type);
	if (fcfg.item_get_value(group_name[2].c_str(), "边框线-横线", block_type) > 0)
		strcpy(mto_10.CBI.h_normal, block_type);
	if (fcfg.item_get_value(group_name[2].c_str(), "边框线-竖线", block_type) > 0)
		strcpy(mto_10.CBI.v_normal, block_type);

	//其他颜色
	if (fcfg.item_get_value(group_name[2].c_str(), "被消除色块-前景色", i) > 0 && i >= -1 && i <= 15)
		remove_fg = i;
	if (fcfg.item_get_value(group_name[2].c_str(), "被消除色块-背景色", i) > 0 && i >= -1 && i <= 15)
		remove_bg = i;

	if (fcfg.item_get_value(group_name[2].c_str(), "选中色块前景色", i) > 0 && i >= 0 && i <= 15)
		bdi_sel_fg = i;
	if (fcfg.item_get_value(group_name[2].c_str(), "选中色块背景色", i) > 0 && i >= -1 && i <= 15)
		bdi_sel_bg = i;

	if (fcfg.item_get_value(group_name[2].c_str(), "反显色块前景色", i) > 0 && i >= 0 && i <= 15)
		bdi_rel_fg = i;
	if (fcfg.item_get_value(group_name[2].c_str(), "反显色块背景色", i) > 0 && i >= -1 && i <= 15)
		bdi_rel_bg = i;

	//[状态栏设置]
	bool top_status = true, low_status = true;
	int normal_fgcolor = -1, normal_bgcolor = -1, catchy_fgcolor = -1, catchy_bgcolor = COLOR_HYELLOW;
	//存在
	if (fcfg.item_get_value(group_name[3].c_str(), "上状态栏", choice) > 0)
		if (choice == 'y' || choice == 'Y')
			top_status = true;
		else if (choice == 'n' || choice == 'N')
			top_status = false;
	gmw_set_status_line_switch(&mto_10, TOP_STATUS_LINE, top_status);

	if (fcfg.item_get_value(group_name[3].c_str(), "下状态栏", choice) > 0)
		if (choice == 'y' || choice == 'Y')
			low_status = true;
		else if (choice == 'n' || choice == 'N')
			low_status = false;
	gmw_set_status_line_switch(&mto_10, LOWER_STATUS_LINE, low_status);

	//颜色
	gmw_set_status_line_color(&mto_10, TOP_STATUS_LINE, normal_bgcolor, normal_fgcolor, catchy_bgcolor, catchy_fgcolor);
	gmw_set_status_line_color(&mto_10, LOWER_STATUS_LINE, normal_bgcolor, normal_fgcolor, catchy_bgcolor, catchy_fgcolor);

}