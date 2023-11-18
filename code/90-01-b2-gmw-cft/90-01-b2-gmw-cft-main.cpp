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
#include "90-01-b2-gmw-cft.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/class_cft.h"
using namespace std;

int main(int argc, char** argv)
{
	int bdi_nor_bg[10] = { 1,2,3,4,5,9,10,11,12,13 }, bdi_nor_fg[10] = { 0 }, bdi_rel_bg = -1, bdi_rel_fg = 7,
		bdi_sel_bg = -1, bdi_sel_fg = 15, remove_bg = -1, remove_fg = -1;

	CONSOLE_GRAPHICS_INFO mto_10;
	gmw_init(&mto_10);
	int dst, arr[8][10] = { 0 }, column = 0, i, j, row, col;
	char mid[8][10], temp[256];
	const char* gname = "合成十";

	if (argc == 3 && !strcmp(argv[1], "--cfgfile"))
	{
		cft fcfg((const char*)argv[2]);
		if (!fcfg.is_open())
		{
			cout << "文件打开失败" << endl;
			return 0;
		}
		cft_file_read(mto_10, fcfg, row, col, dst, bdi_nor_bg, bdi_nor_fg,
			bdi_rel_bg, bdi_rel_fg, bdi_sel_bg, bdi_sel_fg, remove_bg, remove_fg);
		fcfg.close();
	}
	else
	{
		input_init(&row, &col, &dst, mid);
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
	}

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
	{1,  bdi_nor_bg[0], bdi_nor_fg[0],  NULL},	//数字1的“正常”状态
	{2,  bdi_nor_bg[1], bdi_nor_fg[1],  NULL},
	{3,  bdi_nor_bg[2], bdi_nor_fg[2],  NULL},
	{4,  bdi_nor_bg[3], bdi_nor_fg[3],  NULL},
	{5,  bdi_nor_bg[4], bdi_nor_fg[4],  NULL},
	{6,  bdi_nor_bg[5], bdi_nor_fg[5],  NULL},
	{7,  bdi_nor_bg[6], bdi_nor_fg[6],  NULL},
	{8,  bdi_nor_bg[7], bdi_nor_fg[7],  NULL},
	{9,  bdi_nor_bg[8], bdi_nor_fg[8],  NULL},
	{10, bdi_nor_bg[9], bdi_nor_fg[9],  NULL},	//数字10的“正常”状态，如果需要，还可以继续增加11及以后的数字
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1,  bdi_rel_bg == -1 ? bdi_nor_bg[0] : bdi_rel_bg, bdi_rel_fg,  NULL},	//数字1的“可选择+关联”状态
		{2,  bdi_rel_bg == -1 ? bdi_nor_bg[1] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{3,  bdi_rel_bg == -1 ? bdi_nor_bg[2] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{4,  bdi_rel_bg == -1 ? bdi_nor_bg[3] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{5,  bdi_rel_bg == -1 ? bdi_nor_bg[4] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{6,  bdi_rel_bg == -1 ? bdi_nor_bg[5] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{7,  bdi_rel_bg == -1 ? bdi_nor_bg[6] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{8,  bdi_rel_bg == -1 ? bdi_nor_bg[7] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{9,  bdi_rel_bg == -1 ? bdi_nor_bg[8] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{10, bdi_rel_bg == -1 ? bdi_nor_bg[9] : bdi_rel_bg, bdi_rel_fg,  NULL},	//数字10的“可选择+关联”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1, bdi_sel_bg == -1 ? bdi_nor_bg[0] : bdi_sel_bg, bdi_sel_fg, NULL},	//数字1的“选中”状态
		{2, bdi_sel_bg == -1 ? bdi_nor_bg[1] : bdi_sel_bg, bdi_sel_fg, NULL},
		{3, bdi_sel_bg == -1 ? bdi_nor_bg[2] : bdi_sel_bg, bdi_sel_fg, NULL},
		{4, bdi_sel_bg == -1 ? bdi_nor_bg[3] : bdi_sel_bg, bdi_sel_fg, NULL},
		{5, bdi_sel_bg == -1 ? bdi_nor_bg[4] : bdi_sel_bg, bdi_sel_fg, NULL},
		{6, bdi_sel_bg == -1 ? bdi_nor_bg[5] : bdi_sel_bg, bdi_sel_fg, NULL},
		{7, bdi_sel_bg == -1 ? bdi_nor_bg[6] : bdi_sel_bg, bdi_sel_fg, NULL},
		{8, bdi_sel_bg == -1 ? bdi_nor_bg[7] : bdi_sel_bg, bdi_sel_fg, NULL},
		{9, bdi_sel_bg == -1 ? bdi_nor_bg[8] : bdi_sel_bg, bdi_sel_fg, NULL},
		{10,bdi_sel_bg == -1 ? bdi_nor_bg[9] : bdi_sel_bg, bdi_sel_fg, NULL},	//数字10的“选中”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	mto_10.CBI.block_border = 1;

	//90-01-b2-gmw-cft.exe --cfgfile merge_to_10.cfg
	cct_cls();

	//pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;

	array_element(row, col, arr);

	gmw_draw_frame(&mto_10);
	sprintf(temp, "%s-屏幕当前设置为：%d行 %d列", gname, mto_10.lines, mto_10.cols);
	gmw_status_line(&mto_10, TOP_STATUS_LINE, temp);
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(&mto_10, i, j, arr[i][j], bdi_normal);
		}
	gmw_status_line(&mto_10, LOWER_STATUS_LINE, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束");
	game_without_separator(&mto_10, dst, mid, arr, bdi_normal, bdi_related, bdi_selected);

	return 0;
}

