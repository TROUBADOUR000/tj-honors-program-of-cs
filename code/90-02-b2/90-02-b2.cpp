/* 2153592 胡逸凡 信01 */
/* 2048 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>  
#include <time.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

void input_init(int& row, int& col, int& dst, int& delay)
{
	const char gra[8][6] = { "512","1024","2048","4096","8192","16384","32768","65536" };
	CONSOLE_GRAPHICS_INFO background;
	gmw_init(&background);
	gmw_set_frame_color(&background, COLOR_HWHITE, COLOR_HBLACK);
	background.row_num = 8;
	background.col_num = 8;
	gmw_set_frame_style(&background, 10, 3, false);
	gmw_draw_frame(&background);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	char r[6];
	int i, j, start, loop = 1, x, y, X, Y, maction, keycode1, keycode2;
	cct_gotoxy(2, 2);
	cout << "游戏2048参数设置：";
	cct_gotoxy(2, 3);
	cout << "请输入行数[4-8] ";
	cct_getxy(x, y);
	while (1)
	{		
		j = 0;
		while (j < 10)
		{
			cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
			if (j == 0)
				r[0] = keycode1;
			if (!keycode2)
				if (keycode1 == '\r')
					break;
				else
					if (keycode1 > ' ' && keycode1 < 127)
						cout << char(keycode1);
					else
						j--;
			j++;
		}
		if (j == 1 && r[0] >= '4' && r[0] <= '8')
		{
			row = r[0] - '0';
			break;
		}
		cct_gotoxy(x, y);
		cout << setw(11) << setfill(' ') << ' ';
		cct_gotoxy(x, y);
	}
	cct_gotoxy(2, 4);
	cout << "请输入列数[4-10] ";
	cct_getxy(x, y);
	while (1)
	{
		j = 0;
		while (j < 10)
		{
			cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
			if (j <= 1)
				r[j] = keycode1;
			if (!keycode2)
				if (keycode1 == '\r')
					break;
				else
					if (keycode1 > ' ' && keycode1 < 127)
						cout << char(keycode1);
					else
						j--;
			j++;
		}
		if (j == 1 && r[0] >= '4' && r[0] <= '9' || j == 2 && r[0] == '1' && r[1] == '0')
		{
			if (j == 1)
				col = r[0] - '0';
			else
				col = 10;
			break;
		}
		cct_gotoxy(x, y);
		cout << setw(11) << setfill(' ') << ' ';
		cct_gotoxy(x, y);
	}
	cct_gotoxy(2, 5);
	start = (row + col - 1) / 2 - 3;
	start = start > 4 ? 4 : start;
	cout << "请输入目标分数[";
	for (i = start; i < start + 3; i++)
		cout << gra[i] << '/';
	cout << gra[start + 3] << "] ";
	cct_getxy(x, y);
	while (loop)
	{
		loop = 1;
		j = 0;
		while (j < 10)
		{
			cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
			if (j < 5)
				r[j] = keycode1;
			if (!keycode2)
				if (keycode1 == '\r')
				{
					r[j] = '\0';
					break;
				}
				else
					if (keycode1 > ' ' && keycode1 < 127)
						cout << char(keycode1);
					else
						j--;
			j++;
		}
		for (i = start; i < start + 4; i++)
			if (!strcmp(r, gra[i]))
			{
				for (j = 0; gra[i][j] != '\0'; j++)
				{
					dst *= 10;
					dst += gra[i][j] - '0';
				}
				loop = 0;
				break;
			}
		if (loop)
		{
			cct_gotoxy(x, y);
			cout << setw(11) << setfill(' ') << ' ';
			cct_gotoxy(x, y);
		}
	}
	cct_gotoxy(2, 6);
	cout << "请输入动画延时[0-5] ";
	cct_getxy(x, y);
	while (1)
	{
		j = 0;
		while (j < 10)
		{
			cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
			if (j == 0)
				r[0] = keycode1;
			if (!keycode2)
				if (keycode1 == '\r')
					break;
				else
					if (keycode1 > ' ' && keycode1 < 127)
						cout << char(keycode1);
					else
						j--;
			j++;
		}
		if (j == 1 && r[0] >= '0' && r[0] <= '5')
		{
			delay = r[0] - '0';
			break;
		}
		cct_gotoxy(x, y);
		cout << setw(11) << setfill(' ') << ' ';
		cct_gotoxy(x, y);
	}
}

int is_end(int g2048[8][10], int row, int col)
{
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
		{
			if (!g2048[i][j])
				return 0;
			if (i > 0 && j > 0 && i < row - 1 && j < col - 1)
			{
				if (g2048[i][j])
					if (g2048[i][j] == g2048[i - 1][j] || g2048[i][j] == g2048[i + 1][j] || g2048[i][j] == g2048[i][j + 1] ||
						g2048[i][j] == g2048[i][j - 1])
						return 0;
			}
			else if (i == 0)
			{
				if (j == 0)
				{
					if (g2048[i][j] && (g2048[i][j] == g2048[i + 1][j] || g2048[i][j] == g2048[i][j + 1]))
						return 0;
				}
				else if (j == col - 1)
				{
					if (g2048[i][j] && (g2048[i][j] == g2048[i + 1][j] || g2048[i][j] == g2048[i][j - 1]))
						return 0;
				}
				else
					if (g2048[i][j] && (g2048[i][j] == g2048[i + 1][j] || g2048[i][j] == g2048[i][j + 1] || g2048[i][j] == g2048[i][j - 1]))
						return 0;
			}
			else if (i == row - 1)
			{
				if (j == 0)
				{
					if (g2048[i][j] && (g2048[i][j] == g2048[i - 1][j] || g2048[i][j] == g2048[i][j + 1]))
						return 0;
				}
				else if (j == col - 1)
				{
					if (g2048[i][j] && (g2048[i][j] == g2048[i - 1][j] || g2048[i][j] == g2048[i][j - 1]))
						return 0;
				}
				else
					if (g2048[i][j] && (g2048[i][j] == g2048[i - 1][j] || g2048[i][j] == g2048[i][j + 1] || g2048[i][j] == g2048[i][j - 1]))
						return 0;
			}
		}
	return 1;
}

int max_2048(int g2048[8][10])
{
	int i, j, t = 0;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 10; j++)
			if (g2048[i][j] > t)
				t = g2048[i][j];
	return t;
}

void swap(int& a, int& b)
{
	int t;
	t = a;
	a = b;
	b = t;
}

int main()
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{4, COLOR_HRED, COLOR_BLACK, NULL},
		{8, COLOR_GREEN, COLOR_BLACK, NULL},
		{16, COLOR_RED, COLOR_BLACK, NULL},
		{32, COLOR_HGREEN, COLOR_BLACK, NULL},
		{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{128, COLOR_HRED, COLOR_BLACK, NULL},
		{256, COLOR_HWHITE, COLOR_BLACK, NULL},
		{512, COLOR_HBLACK, COLOR_BLACK, NULL},
		{1024, COLOR_HPINK, COLOR_BLACK, NULL},
		{2048, COLOR_WHITE, COLOR_BLACK, NULL},
		{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
		{8192, COLOR_PINK, COLOR_BLACK, NULL},
		{16384, COLOR_HBLUE, COLOR_BLACK, NULL},
		{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
		{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	char temp[256];
	int row, col, g2048[8][10] = { 0 }, dst = 0, delay = 0, sum = 0, r, c, i, j, k, loop = 1;
	int MAction, MRow = 0, MCol = 0, KeyCode1, KeyCode2, is_quit = 0, game_over = 1;
	CONSOLE_GRAPHICS_INFO p2048;
	gmw_init(&p2048);
	gmw_set_color(&p2048, COLOR_BLACK, COLOR_HWHITE);			//整个窗口颜色
	gmw_set_font(&p2048, "新宋体", 16);						//字体
	gmw_set_frame_style(&p2048, 10, 5, true);					//游戏主区域风格：每个色块宽10高5，有分隔线【数字色块带边框，宽度为10(放最多6位数字)，高度为5(为了保持色块为方形)】
	gmw_set_frame_color(&p2048, COLOR_HWHITE, COLOR_BLACK);	//游戏主区域颜色
	gmw_set_block_border_switch(&p2048, true);				//小色块带边框
	gmw_set_ext_rowcol(&p2048, 1, 1, 0, 0);
	gmw_set_rowno_switch(&p2048, false);		//不显示行号
	gmw_set_colno_switch(&p2048, false);		//不显示列标
	gmw_set_status_line_color(&p2048, TOP_STATUS_LINE, COLOR_HWHITE, COLOR_BLACK, COLOR_HWHITE, COLOR_HRED);
	while (game_over)
	{
		is_quit = 0;
		input_init(row, col, dst, delay);
		gmw_set_delay(&p2048, 2, delay * 5);
		gmw_set_rowcol(&p2048, row, col);
		srand((unsigned)time(NULL));
		for (i = 0; i < 2; i++)
		{
			r = rand() % row;
			c = rand() % col;
			if (!g2048[r][c])
				g2048[r][c] = 2;
			else
				i--;
		}
		gmw_draw_frame(&p2048);
		while (!is_quit)
		{
			loop = 1;
			sprintf(temp, "目标:%d 分数:%d (R:重玩 Q:退出)", dst, sum);
			gmw_status_line(&p2048, TOP_STATUS_LINE, temp);
			for (i = 0; i < row; i++)
				for (j = 0; j < col; j++)
					gmw_draw_block(&p2048, i, j, g2048[i][j], bdi_normal);
			cct_setcursor(CURSOR_INVISIBLE);
			while (loop)
			{
				if (gmw_read_keyboard_and_mouse(&p2048, MAction, MRow, MCol, KeyCode1, KeyCode2) == CCT_KEYBOARD_EVENT)
				{
					switch (KeyCode1)
					{
					case 'r':
					case 'R':
						is_quit = 1;
						loop = 0;
						break;
					case 'q':
					case 'Q':
						return -1;
					case 224:
						switch (KeyCode2)
						{
						case KB_ARROW_UP:
							for (j = 0; j < col; j++)
							{
								for (i = 0; i < row; i++)
									if (g2048[i][j])
										for (k = 0; k < i; k++)
											if (!g2048[k][j])
											{
												gmw_move_block(&p2048, i, j, g2048[i][j], 0, bdi_normal, DOWN_TO_UP, i - k);
												swap(g2048[i][j], g2048[k][j]);
												loop = 0;
												break;
											}
								for (i = 0; i < row - 1; i++)
									if (!g2048[i][j])
										break;
									else
										if (g2048[i][j] == g2048[i + 1][j])
										{
											g2048[i][j] *= 2;
											sum += g2048[i][j];
											g2048[i + 1][j] = 0;
											loop = 0;
											gmw_draw_block(&p2048, i + 1, j, 0, bdi_normal);
											gmw_draw_block(&p2048, i, j, g2048[i][j], bdi_normal);
											if (i + 2 < row)
												for (k = i + 2; k < row; k++)
													if (g2048[k][j])
													{
														gmw_move_block(&p2048, k, j, g2048[k][j], 0, bdi_normal, DOWN_TO_UP, 1);
														swap(g2048[k - 1][j], g2048[k][j]);
													}
										}
							}							
							break;
						case KB_ARROW_DOWN:
							for (j = 0; j < col; j++)
							{
								for (i = row - 1; i >= 0; i--)
									if (g2048[i][j])
										for (k = row - 1; k > i; k--)
											if (!g2048[k][j])
											{
												gmw_move_block(&p2048, i, j, g2048[i][j], 0, bdi_normal, UP_TO_DOWN, k - i);
												swap(g2048[i][j], g2048[k][j]);
												loop = 0;
												break;
											}
								for (i = row - 1; i > 0; i--)
									if (!g2048[i][j])
										break;
									else
										if (g2048[i][j] == g2048[i - 1][j])
										{
											g2048[i][j] *= 2;
											sum += g2048[i][j];
											g2048[i - 1][j] = 0;
											loop = 0;
											gmw_draw_block(&p2048, i - 1, j, 0, bdi_normal);
											gmw_draw_block(&p2048, i, j, g2048[i][j], bdi_normal);
											if (i - 2 >= 0)
												for (k = i - 2; k >= 0; k--)
													if (g2048[k][j])
													{
														gmw_move_block(&p2048, k, j, g2048[k][j], 0, bdi_normal, UP_TO_DOWN, 1);
														swap(g2048[k + 1][j], g2048[k][j]);
													}
										}
							}
							break;
						case KB_ARROW_LEFT:
							for (i = 0; i < row; i++)
							{
								for (j = 0; j < col; j++)
									if (g2048[i][j])
										for (k = 0; k < j; k++)
											if (!g2048[i][k])
											{
												gmw_move_block(&p2048, i, j, g2048[i][j], 0, bdi_normal, RIGHT_TO_LEFT, j - k);
												swap(g2048[i][j], g2048[i][k]);
												loop = 0;
												break;
											}
								for (j = 0; j < col - 1; j++)
									if (!g2048[i][j])
										break;
									else
										if (g2048[i][j] == g2048[i][j + 1])
										{
											g2048[i][j] *= 2;
											sum += g2048[i][j];
											g2048[i][j + 1] = 0;
											loop = 0;
											gmw_draw_block(&p2048, i, j + 1, 0, bdi_normal);
											gmw_draw_block(&p2048, i, j, g2048[i][j], bdi_normal);
											if (j + 2 < col)
												for (k = j + 2; k < col; k++)
													if (g2048[i][k])
													{
														gmw_move_block(&p2048, i, k, g2048[i][k], 0, bdi_normal, RIGHT_TO_LEFT, 1);
														swap(g2048[i][k], g2048[i][k - 1]);
													}
										}
							}
							break;
						case KB_ARROW_RIGHT:
							for (i = 0; i < row; i++)
							{
								for (j = col - 1; j >= 0; j--)
									if (g2048[i][j])
										for (k = col - 1; k > j; k--)
											if (!g2048[i][k])
											{
												gmw_move_block(&p2048, i, j, g2048[i][j], 0, bdi_normal, LEFT_TO_RIGHT, k - j);
												swap(g2048[i][j], g2048[i][k]);
												loop = 0;
												break;
											}
								for (j = col - 1; j > 0; j--)
									if (!g2048[i][j])
										break;
									else
										if (g2048[i][j] == g2048[i][j - 1])
										{
											g2048[i][j] *= 2;
											sum += g2048[i][j];
											g2048[i][j - 1] = 0;
											loop = 0;
											gmw_draw_block(&p2048, i, j - 1, 0, bdi_normal);
											gmw_draw_block(&p2048, i, j, g2048[i][j], bdi_normal);
											if (j - 2 >= 0)
												for (k = j - 2; k >= 0; k--)
													if (g2048[i][k])
													{
														gmw_move_block(&p2048, i, k, g2048[i][k], 0, bdi_normal, LEFT_TO_RIGHT, 1);
														swap(g2048[i][k], g2048[i][k + 1]);
													}
										}
							}
							break;
						}

					}
				}
			}
			cct_setcursor(CURSOR_VISIBLE_NORMAL);
			if (!is_quit)
			{
				if (max_2048(g2048) == dst)
				{
					game_over = 0;
					gmw_status_line(&p2048, TOP_STATUS_LINE, NULL, "游戏结束！！！");
					break;
				}
				while (1)
				{
					r = rand() % row;
					c = rand() % col;
					if (!g2048[r][c])
					{
						g2048[r][c] = rand() % 2 == 0 ? 2 : 4;
						break;
					}
				}
				gmw_draw_block(&p2048, r, c, g2048[r][c], bdi_normal);
				cout << is_end(g2048, p2048.row_num, p2048.col_num);
				if (is_end(g2048, p2048.row_num, p2048.col_num))
				{
					is_quit = 1;
					gmw_status_line(&p2048, TOP_STATUS_LINE, NULL, "无法合成，游戏结束，按回车键重新开始");
					while (_getch() != '\r');
				}
			}
		}
	}
	return 0;
}