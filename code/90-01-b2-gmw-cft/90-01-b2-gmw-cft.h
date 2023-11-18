/* 2153592 ºúÒÝ·² ÐÅ01 */
#pragma once
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/class_cft.h"

void array_element(int row, int col, int arr[10][10]);
void input_init(int* row, int* col, int* dst, char mid[10][10]);
int recursion_judge(int num, char a, int arr[10][10], char mid[10][10], int row, int col);
int is_end(int arr[10][10], int row, int col);

void restore_mid(char mid[8][10]);
int max_arr(int arr[8][10], int col, int row);
int num_mine(int row, int col, char mid[8][10]);
int is_end(int arr[8][10], int row, int col);

void swap(int* a, int* b);

void game_without_separator(CONSOLE_GRAPHICS_INFO* mto_10, int dst, char mid[8][10], int arr[8][10],
	const BLOCK_DISPLAY_INFO bdi_normal[], const BLOCK_DISPLAY_INFO bdi_related[], const BLOCK_DISPLAY_INFO bdi_selected[]);

void cft_file_read(CONSOLE_GRAPHICS_INFO& mto_10, cft& file, int& row, int& col, int& dst, int bdi_nor_bg[10],
	int bdi_nor_fg[10], int& bdi_rel_bg, int& bdi_rel_fg, int& bdi_sel_bg, int& bdi_sel_fg, int& remove_bg, int& remove_fg);