/* 2153592 ���ݷ� ��01 */
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
	const char* gname = "�ϳ�ʮ";

	if (argc == 3 && !strcmp(argv[1], "--cfgfile"))
	{
		cft fcfg((const char*)argv[2]);
		if (!fcfg.is_open())
		{
			cout << "�ļ���ʧ��" << endl;
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
		gmw_set_rowno_switch(&mto_10, true);//��ʾ�к�
		gmw_set_colno_switch(&mto_10, true);//��ʾ�б�
		gmw_set_font(&mto_10, "Terminal", 20, 10);
		gmw_set_frame_style(&mto_10, 6, 3, false);//�޷ָ���
		gmw_set_ext_rowcol(&mto_10, 2, 3, 4, 5);	//�������У���2��3����4��5
		gmw_set_rowcol(&mto_10, row, col);
	}

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
	{1,  bdi_nor_bg[0], bdi_nor_fg[0],  NULL},	//����1�ġ�������״̬
	{2,  bdi_nor_bg[1], bdi_nor_fg[1],  NULL},
	{3,  bdi_nor_bg[2], bdi_nor_fg[2],  NULL},
	{4,  bdi_nor_bg[3], bdi_nor_fg[3],  NULL},
	{5,  bdi_nor_bg[4], bdi_nor_fg[4],  NULL},
	{6,  bdi_nor_bg[5], bdi_nor_fg[5],  NULL},
	{7,  bdi_nor_bg[6], bdi_nor_fg[6],  NULL},
	{8,  bdi_nor_bg[7], bdi_nor_fg[7],  NULL},
	{9,  bdi_nor_bg[8], bdi_nor_fg[8],  NULL},
	{10, bdi_nor_bg[9], bdi_nor_fg[9],  NULL},	//����10�ġ�������״̬�������Ҫ�������Լ�������11���Ժ������
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1,  bdi_rel_bg == -1 ? bdi_nor_bg[0] : bdi_rel_bg, bdi_rel_fg,  NULL},	//����1�ġ���ѡ��+������״̬
		{2,  bdi_rel_bg == -1 ? bdi_nor_bg[1] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{3,  bdi_rel_bg == -1 ? bdi_nor_bg[2] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{4,  bdi_rel_bg == -1 ? bdi_nor_bg[3] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{5,  bdi_rel_bg == -1 ? bdi_nor_bg[4] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{6,  bdi_rel_bg == -1 ? bdi_nor_bg[5] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{7,  bdi_rel_bg == -1 ? bdi_nor_bg[6] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{8,  bdi_rel_bg == -1 ? bdi_nor_bg[7] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{9,  bdi_rel_bg == -1 ? bdi_nor_bg[8] : bdi_rel_bg, bdi_rel_fg,  NULL},
		{10, bdi_rel_bg == -1 ? bdi_nor_bg[9] : bdi_rel_bg, bdi_rel_fg,  NULL},	//����10�ġ���ѡ��+������״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1, bdi_sel_bg == -1 ? bdi_nor_bg[0] : bdi_sel_bg, bdi_sel_fg, NULL},	//����1�ġ�ѡ�С�״̬
		{2, bdi_sel_bg == -1 ? bdi_nor_bg[1] : bdi_sel_bg, bdi_sel_fg, NULL},
		{3, bdi_sel_bg == -1 ? bdi_nor_bg[2] : bdi_sel_bg, bdi_sel_fg, NULL},
		{4, bdi_sel_bg == -1 ? bdi_nor_bg[3] : bdi_sel_bg, bdi_sel_fg, NULL},
		{5, bdi_sel_bg == -1 ? bdi_nor_bg[4] : bdi_sel_bg, bdi_sel_fg, NULL},
		{6, bdi_sel_bg == -1 ? bdi_nor_bg[5] : bdi_sel_bg, bdi_sel_fg, NULL},
		{7, bdi_sel_bg == -1 ? bdi_nor_bg[6] : bdi_sel_bg, bdi_sel_fg, NULL},
		{8, bdi_sel_bg == -1 ? bdi_nor_bg[7] : bdi_sel_bg, bdi_sel_fg, NULL},
		{9, bdi_sel_bg == -1 ? bdi_nor_bg[8] : bdi_sel_bg, bdi_sel_fg, NULL},
		{10,bdi_sel_bg == -1 ? bdi_nor_bg[9] : bdi_sel_bg, bdi_sel_fg, NULL},	//����10�ġ�ѡ�С�״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	mto_10.CBI.block_border = 1;

	//90-01-b2-gmw-cft.exe --cfgfile merge_to_10.cfg
	cct_cls();

	//pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;

	array_element(row, col, arr);

	gmw_draw_frame(&mto_10);
	sprintf(temp, "%s-��Ļ��ǰ����Ϊ��%d�� %d��", gname, mto_10.lines, mto_10.cols);
	gmw_status_line(&mto_10, TOP_STATUS_LINE, temp);
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(&mto_10, i, j, arr[i][j], bdi_normal);
		}
	gmw_status_line(&mto_10, LOWER_STATUS_LINE, "��ͷ��/����ƶ����س���/�������ѡ��Q/�����Ҽ�����");
	game_without_separator(&mto_10, dst, mid, arr, bdi_normal, bdi_related, bdi_selected);

	return 0;
}

