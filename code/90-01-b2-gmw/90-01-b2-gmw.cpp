/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include"90-01-b2-gmw.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

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

int main()
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1,  COLOR_BLUE,   COLOR_BLACK,  NULL},	//数字1的“正常”状态
		{2,  COLOR_GREEN,  COLOR_BLACK,  NULL},
		{3,  COLOR_CYAN,   COLOR_BLACK,  NULL},
		{4,  COLOR_RED,    COLOR_BLACK,  NULL},
		{5,  COLOR_PINK,   COLOR_BLACK,  NULL},
		{6,  COLOR_HBLUE,  COLOR_BLACK,  NULL},
		{7,  COLOR_HGREEN, COLOR_BLACK,  NULL},
		{8,  COLOR_HCYAN,  COLOR_BLACK,  NULL},
		{9,  COLOR_HRED,   COLOR_BLACK,  NULL},
		{10, COLOR_HPINK,  COLOR_BLACK,  NULL},	//数字10的“正常”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1,  COLOR_BLUE,   COLOR_WHITE,  NULL},	//数字1的“可选择+关联”状态
		{2,  COLOR_GREEN,  COLOR_WHITE,  NULL},
		{3,  COLOR_CYAN,   COLOR_WHITE,  NULL},
		{4,  COLOR_RED,    COLOR_WHITE,  NULL},
		{5,  COLOR_PINK,   COLOR_WHITE,  NULL},
		{6,  COLOR_HBLUE,  COLOR_WHITE,  NULL},
		{7,  COLOR_HGREEN, COLOR_WHITE,  NULL},
		{8,  COLOR_HCYAN,  COLOR_WHITE,  NULL},
		{9,  COLOR_HRED,   COLOR_WHITE,  NULL},
		{10, COLOR_HPINK,  COLOR_WHITE,  NULL},	//数字10的“可选择+关联”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1, COLOR_BLUE,   COLOR_HWHITE, NULL},	//数字1的“选中”状态
		{2, COLOR_GREEN,  COLOR_HWHITE, NULL},
		{3, COLOR_CYAN,   COLOR_HWHITE, NULL},
		{4, COLOR_RED,    COLOR_HWHITE, NULL},
		{5, COLOR_PINK,   COLOR_HWHITE, NULL},
		{6, COLOR_HBLUE,  COLOR_HWHITE, NULL},
		{7, COLOR_HGREEN, COLOR_HWHITE, NULL},
		{8, COLOR_HCYAN,  COLOR_HWHITE, NULL},
		{9, COLOR_HRED,   COLOR_HWHITE, NULL},
		{10,COLOR_HPINK,  COLOR_HWHITE, NULL},	//数字10的“选中”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	cout << "伪图形界面完整版（无分隔线）" << endl;
	int dst, arr[8][10] = { 0 }, column = 0, i, j, row, col;
	char mid[8][10], temp[256];
	const char* gname = "合成十";
	input_init(&row, &col, &dst, mid, 4);
	cct_cls();
	array_element(row, col, arr);

	CONSOLE_GRAPHICS_INFO mto_10;
	gmw_init(&mto_10);
	gmw_set_color(&mto_10, COLOR_BLACK, COLOR_WHITE);
	gmw_set_frame_color(&mto_10, COLOR_WHITE, COLOR_BLACK);
	gmw_set_block_border_switch(&mto_10, true);
	gmw_set_rowno_switch(&mto_10, true);//显示行号
	gmw_set_colno_switch(&mto_10, true);//显示列标
	gmw_set_font(&mto_10, "Terminal", 20, 10);
	gmw_set_frame_style(&mto_10, 6, 3, false);//无分隔线
	gmw_set_ext_rowcol(&mto_10, 2, 3, 4, 5);	//额外行列：上2下3，左4右5
	gmw_set_rowcol(&mto_10, row, col);
	gmw_draw_frame(&mto_10);
	sprintf(temp, "%s-屏幕当前设置为：%d行 %d列", gname, mto_10.lines, mto_10.cols);
	gmw_status_line(&mto_10, TOP_STATUS_LINE, temp);
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(&mto_10, i, j, arr[i][j], bdi_normal);
		}
	gmw_status_line(&mto_10, LOWER_STATUS_LINE, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束");
	game_without_separator(&mto_10, dst, mid, arr, bdi_normal, bdi_related, bdi_selected);
}