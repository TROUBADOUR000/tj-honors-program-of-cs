/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


/* --------------------------------------------------
		此处可以给出需要的内部辅助工具函数
		1、函数名不限，建议为 gmw_inner_* 
		2、个数不限
		3、必须是static函数，确保只在本源文件中使用
   -------------------------------------------------- */


/* ----------------------------------------------- 
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
            2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->col_num = col;
	pCGI->row_num = row;
	pCGI->CFI.bhigh = pCGI->CFI.block_high * row + (row - 1) * pCGI->CFI.separator + 2;
	pCGI->CFI.bwidth = (pCGI->CFI.block_width + 1) / 2 * 2 * col + (col - 1) * 2 * pCGI->CFI.separator + 4;
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line
		+ pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->draw_frame_with_col_no;
	pCGI->SLI.width = pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
				    前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if (cascade)
	{
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;                         //游戏主框架区域
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;            //状态栏信息		
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;            //状态栏醒目
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
            2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	if (!strcmp(fontname, "Terminal") || !strcmp(fontname, "新宋体"))
		strcpy(pCGI->CFT.font_type, fontname);
	else
		return -1;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int d = delay_ms >= 0 ? delay_ms : 0;
	switch (type)
	{
	case 0:
		pCGI->delay_of_draw_frame = d;
		break;
	case 1:
		pCGI->delay_of_draw_block = d;
		break;
	case 2:
		pCGI->delay_of_block_moved = d;
		break;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->extern_down_lines = down_lines;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;
	pCGI->extern_up_lines = up_lines;
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line
		+ pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->draw_frame_with_col_no;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1; 
	/*	╔ ═ ╤ ═ ╗
    	║          ║
	    ╟ ═ ┼ ═ ╢
	    ║          ║
	    ╚ ═ ╧ ═ ╝ */
	char tabs[4][11][3] =
	{ {"╔","╚","╗","╝","═","║","╦","╩","╠","╣","╬"},
	  {"┌","└","┐","┘","─","│","┬","┴","├","┤","┼"},
	  {"╒","╘","╕","╛","═","│","╤","╧","╞","╡","╪"},
	  {"╓","╙","╖","╜","─","║","╥","╨","╟","╢","╫"} };
	strcpy(pCGI->CFI.top_left, tabs[type - 1][0]);
	strcpy(pCGI->CFI.lower_left, tabs[type - 1][1]);
	strcpy(pCGI->CFI.top_right, tabs[type - 1][2]);
	strcpy(pCGI->CFI.lower_right, tabs[type - 1][3]);
	strcpy(pCGI->CFI.h_normal, tabs[type - 1][4]);
	strcpy(pCGI->CFI.v_normal, tabs[type - 1][5]);
	strcpy(pCGI->CFI.h_top_separator, tabs[type - 1][6]);
	strcpy(pCGI->CFI.h_lower_separator, tabs[type - 1][7]);
	strcpy(pCGI->CFI.v_left_separator, tabs[type - 1][8]);
	strcpy(pCGI->CFI.v_right_separator, tabs[type - 1][9]);
	strcpy(pCGI->CFI.mid_separator, tabs[type - 1][10]);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal, const char* h_top_separator,
	const char* h_lower_separator, const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	strcpy(pCGI->CFI.top_left, top_left);
	strcpy(pCGI->CFI.lower_left, lower_left);
	strcpy(pCGI->CFI.top_right, top_right);
	strcpy(pCGI->CFI.lower_right, lower_right);
	strcpy(pCGI->CFI.h_normal, h_normal);
	strcpy(pCGI->CFI.v_normal, v_normal);
	strcpy(pCGI->CFI.h_top_separator, h_top_separator);
	strcpy(pCGI->CFI.h_lower_separator, h_lower_separator);
	strcpy(pCGI->CFI.v_left_separator, v_left_separator);
	strcpy(pCGI->CFI.v_right_separator, v_right_separator);
	strcpy(pCGI->CFI.mid_separator, mid_separator);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.block_width = block_width;
	pCGI->CFI.separator = separator;
	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + (pCGI->row_num - 1) * pCGI->CFI.separator + 2;
	pCGI->CFI.bwidth = (pCGI->CFI.block_width + 1) / 2 * 2 * pCGI->col_num + (pCGI->col_num - 1) * 2 * pCGI->CFI.separator + 4;
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line
		+ pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->draw_frame_with_col_no;
	pCGI->SLI.width = pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFI.bgcolor = bgcolor == -1 ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = fgcolor == -1 ? pCGI->area_fgcolor : fgcolor;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1; 
    /*  ╔ ═ ╗
    	║    ║
    	╚ ═ ╝ */
	char tabs[4][6][3] =
	{ {"╔","╚","╗","╝","═","║"},
	  {"┌","└","┐","┘","─","│"},
	  {"╒","╘","╕","╛","═","│"},
	  {"╓","╙","╖","╜","─","║"} };
	strcpy(pCGI->CBI.top_left, tabs[type - 1][0]);
	strcpy(pCGI->CBI.lower_left, tabs[type - 1][1]);
	strcpy(pCGI->CBI.top_right, tabs[type - 1][2]);
	strcpy(pCGI->CBI.lower_right, tabs[type - 1][3]);
	strcpy(pCGI->CBI.h_normal, tabs[type - 1][4]);
	strcpy(pCGI->CBI.v_normal, tabs[type - 1][5]);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	strcpy(pCGI->CBI.top_left, top_left);
	strcpy(pCGI->CBI.lower_left, lower_left);
	strcpy(pCGI->CBI.top_right, top_right);
	strcpy(pCGI->CBI.lower_right, lower_right);
	strcpy(pCGI->CBI.h_normal, h_normal);
	strcpy(pCGI->CBI.v_normal, v_normal);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type == 1)
	{
		pCGI->SLI.is_lower_status_line = on_off;
		pCGI->lower_status_line = on_off;
	}
	else if (type == 0)
	{
		pCGI->SLI.is_top_status_line = on_off;
		pCGI->top_status_line = on_off;
	}
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line
		+ pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->draw_frame_with_col_no;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type == 1)
	{
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	else if (type == 0)
	{
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_row_no = on_off;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;
	pCGI->SLI.width = pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_col_no = on_off;	
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line
		+ pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->draw_frame_with_col_no;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记 */
	pCGI->inited = CGI_INITED;
	gmw_set_rowcol(pCGI, row, col);
	gmw_set_color(pCGI);
	gmw_set_font(pCGI);
	gmw_set_ext_rowcol(pCGI, 0, 0, 0, 0);
	gmw_set_status_line_switch(pCGI, 0);
	gmw_set_status_line_switch(pCGI, 1);
	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);
	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_frame_style(pCGI, 2, 1, 1);
	gmw_set_delay(pCGI, 0, 0);
	gmw_set_delay(pCGI, 1, 0);
	gmw_set_delay(pCGI, 2, 3);
	gmw_set_block_default_linetype(pCGI, 1);
	gmw_set_frame_color(pCGI);
	gmw_set_block_border_switch(pCGI, 0);
	pCGI->SLI.top_catchy_fgcolor = 14;
	pCGI->SLI.lower_catchy_fgcolor = 14;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int i, j, k, r;
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high);
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	cct_cls();
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	for (i = 0; i < pCGI->lines; i++)
		cout << setw(pCGI->cols) << setfill(' ') << ' ';
	if (pCGI->draw_frame_with_col_no && pCGI->CFI.separator)
	{
		cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
		cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.block_width / 2 + 1,
			pCGI->extern_up_lines + pCGI->SLI.is_top_status_line);
		cout << '0';
		for (i = 1; i < pCGI->col_num; i++)
			if (i < 100)
				cout << setw(pCGI->CFI.block_width + 2) << setfill(' ') << i;
			else
				cout << setw(pCGI->CFI.block_width + 2) << setfill(' ') << "**";
	}
	else if (pCGI->draw_frame_with_col_no)
	{
		cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
		cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.block_width / 2 + 1,
			pCGI->extern_up_lines + pCGI->SLI.is_top_status_line);
		cout << '0';
		for (i = 1; i < pCGI->col_num; i++)
			if (i < 100)
				cout << setw(pCGI->CFI.block_width) << setfill(' ') << i;
			else
				cout << setw(pCGI->CFI.block_width) << setfill(' ') << "**";
	}
	cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	for (i = 0; i < pCGI->row_num; i++)
		for (j = 0; j < pCGI->CFI.block_high + 1; j++)
		{
			if (pCGI->CFI.separator)
			{
				cct_gotoxy(pCGI->extern_left_cols, j + i * (pCGI->CFI.block_high + 1)
					+ pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + +pCGI->draw_frame_with_col_no);
				if (pCGI->draw_frame_with_row_no && j == pCGI->CFI.block_high / 2 + 1)
				{
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
					if (i < 26)
						cout << (char)('A' + i);
					else if (i < 52)
						cout << (char)('a' + i - 26);
					else
						cout << '*';
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
				cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, j + i * (1 + pCGI->CFI.block_high) +
					pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no);
				for (k = 0; k < pCGI->col_num; k++)
				{
					Sleep(pCGI->delay_of_draw_frame);
					for (r = 0; r < pCGI->CFI.block_width + 2; r += 2)
						if (j == 0)
							if (r == 0)
								if (i == 0)
									cout << (k == 0 ? pCGI->CFI.top_left : pCGI->CFI.h_top_separator);
								else
									cout << (k == 0 ? pCGI->CFI.v_left_separator : pCGI->CFI.mid_separator);
							else
								cout << pCGI->CFI.h_normal;
						else
							cout << (r == 0 ? pCGI->CFI.v_normal : "  ");
				}
				if (j == 0)
					cout << (i == 0 ? pCGI->CFI.top_right : pCGI->CFI.v_right_separator);
				else
					cout << pCGI->CFI.v_normal;
			}
			else
			{
				cct_gotoxy(pCGI->extern_left_cols, j + i * pCGI->CFI.block_high
					+ pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no);
				if (pCGI->draw_frame_with_row_no && j == (pCGI->CFI.block_high + 1) / 2)
				{
					cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
					if (i < 26)
						cout << (char)('A' + i);
					else if (i < 52)
						cout << (char)('a' + i - 26);
					else
						cout << '*';
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
				cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, j + i * pCGI->CFI.block_high
					+ pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no);
				if (i == 0 && j == 0)
				{
					cout << pCGI->CFI.top_left;
					for (r = 0; r < pCGI->CFI.bwidth - 4; r += 2)
						cout << pCGI->CFI.h_normal;
					cout << pCGI->CFI.top_right;
				}
				else if (j != 0)
				{
					cout << pCGI->CFI.v_normal;
					cout << setw(pCGI->CFI.bwidth - 4) << setfill(' ') << ' ';
					cout << pCGI->CFI.v_normal;
				}
			}
			Sleep(pCGI->delay_of_draw_frame);
		}
	cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->extern_up_lines +
		pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->CFI.bhigh - 1);
	if (pCGI->CFI.separator)
		for (i = 0; i < pCGI->col_num; i++)
		{
			for (j = 0; j < pCGI->CFI.block_width + 2; j += 2)
				if (j == 0)
					cout << (i == 0 ? pCGI->CFI.lower_left : pCGI->CFI.h_lower_separator);
				else
					cout << pCGI->CFI.h_normal;
			Sleep(pCGI->delay_of_draw_frame);
		}
	else
	{
		cout << pCGI->CFI.lower_left;
		for (r = 0; r < pCGI->CFI.bwidth - 4; r += 2)
			cout << pCGI->CFI.h_normal;
	}
	cout << pCGI->CFI.lower_right;
	cct_setcolor();
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const char* msg, const char* catchy_msg)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int length_catchy, length_msg, num = 0, i;
	char m[256] = { '\0' }, cm[256] = { '\0' };
	if (catchy_msg)
	{
		length_catchy = int(strlen(catchy_msg));
		for (i = 0; catchy_msg[i] != '\0'; i++)
			cm[i] = catchy_msg[i];
	}
	else
	{
		length_catchy = 0;
		cm[0] = '\0';
	}
	if (msg)
	{
		length_msg = int(strlen(msg));
		for (i = 0; msg[i] != '\0'; i++)
			m[i] = msg[i];
	}
	else
	{
		length_msg = 0;
		m[0] = '\0';
	}
	if (length_catchy > pCGI->SLI.width)
	{
		m[0] = '\0';
		cm[pCGI->SLI.width] = '\0';
		for (i = 0; cm[i] != '\0'; i++)
			if (cm[i] < 0)
				num++;
		if (num % 2 == 1)
			cm[pCGI->SLI.width - 1] = '\0';
	}
	else if (length_catchy + length_msg > pCGI->SLI.width)
	{
		m[pCGI->SLI.width - length_catchy] = '\0';
		for (i = 0; m[i] != '\0'; i++)
			if (m[i] < 0)
				num++;
		if (num % 2 == 1)
			cm[pCGI->SLI.width - 1 - length_catchy] = '\0';
	}
	if (type == 0 && pCGI->SLI.is_top_status_line)
	{
		if (cm[0] != '\0')
			cct_showstr(pCGI->extern_left_cols, pCGI->extern_up_lines, cm, pCGI->SLI.top_catchy_bgcolor,
				pCGI->SLI.top_catchy_fgcolor, 1, pCGI->SLI.width);
		if (m[0] != '\0')
			cct_showstr(pCGI->extern_left_cols + length_catchy, pCGI->extern_up_lines, m, pCGI->SLI.top_normal_bgcolor,
				pCGI->SLI.top_normal_fgcolor, 1, pCGI->SLI.width - length_catchy);
	}
	else if (type == 1 && pCGI->SLI.is_lower_status_line)
	{
		if (cm[0] != '\0')
			cct_showstr(pCGI->extern_left_cols, pCGI->SLI.lower_start_y, cm, pCGI->SLI.lower_catchy_bgcolor, 
				pCGI->SLI.lower_catchy_fgcolor, 1, pCGI->SLI.width);
		if (m[0] != '\0')
			cct_showstr(pCGI->extern_left_cols + length_catchy, pCGI->SLI.lower_start_y, m, pCGI->SLI.lower_normal_bgcolor, 
				pCGI->SLI.lower_normal_fgcolor, 1,pCGI->SLI.width - length_catchy);
	}
	cct_setcolor();
	cct_gotoxy(0, pCGI->lines - 4);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int x, y, t, h;
	for (t = 0; bdi[t].value != BDI_VALUE_END; t++)
		if (bdi_value == bdi[t].value)
			break;
	if (bdi[t].value == BDI_VALUE_END)
		return 0;
	x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + 2 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
	y = pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + 1 + row_no * (pCGI->CFI.separator + pCGI->CFI.block_high)
		+ pCGI->CFI.block_high / 2;
	h = (pCGI->CFI.block_high + 1) / 2;
	cct_setcolor(bdi[t].bgcolor == -1 ? pCGI->CFI.bgcolor : bdi[t].bgcolor,
		bdi[t].fgcolor == -1 ? pCGI->CFI.fgcolor : bdi[t].fgcolor);
	if (bdi[t].value != 0)
		if (pCGI->CBI.block_border)
		{
			for (int j = h - 1; j > 0; j--)
			{
				cct_gotoxy(x, y - j);
				if (j == h - 1)
				{
					cout << pCGI->CBI.top_left;
					for (int i = 0; i < pCGI->CFI.block_width - 4; i += 2)
						cout << pCGI->CBI.h_normal;
					cout << pCGI->CBI.top_right;
				}
				else
				{
					cout << pCGI->CBI.v_normal;
					cout << setw(pCGI->CFI.block_width - 4) << setfill(' ') << ' ';
					cout << pCGI->CBI.v_normal;
				}
			}
			cct_gotoxy(x, y);
			cout << pCGI->CBI.v_normal;
			if (bdi[t].content == NULL)
				cout << setw(pCGI->CFI.block_width - 4) << setiosflags(ios::left) << bdi_value;
			else
				cout << bdi[t].content;
			cct_gotoxy(x + pCGI->CFI.block_width - 2, y);
			cout << pCGI->CBI.v_normal;
			for (int j = 1; j < h; j++)
			{
				cct_gotoxy(x, y + j);
				if (j == h - 1)
				{
					cout << pCGI->CBI.lower_left;
					for (int i = 0; i < pCGI->CFI.block_width - 4; i += 2)
						cout << pCGI->CBI.h_normal;
					cout << pCGI->CBI.lower_right;
				}
				else
				{
					cout << pCGI->CBI.v_normal;
					cout << setw(pCGI->CFI.block_width - 4) << setfill(' ') << ' ';
					cout << pCGI->CBI.v_normal;
				}
			}
			Sleep(pCGI->delay_of_draw_block);
		}
		else
		{
			cct_gotoxy(x, y);
			if (bdi[t].content == NULL)
				cout << setw(pCGI->CFI.block_width - 2) << bdi_value;
			else
				cout << bdi[t].content;
		}
	else
		if (pCGI->CBI.block_border)
		{
			for (int j = y - h + 1; j < y + h; j++)
			{
				cct_gotoxy(x, j);
				cout << setw(pCGI->CFI.block_width) << setfill(' ') << ' ';
			}
			Sleep(pCGI->delay_of_draw_block);
		}
		else
		{
			cct_gotoxy(x, y);
			cout << setw(pCGI->CFI.block_width) << setfill(' ') << ' ';
		}
	cct_setcolor();
	cct_gotoxy(0, pCGI->lines - 4);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int l, x, y, h, t;
	for (t = 0; bdi[t].value != BDI_VALUE_END; t++)
		if (bdi_value == bdi[t].value)
			break;
	if (bdi[t].value == BDI_VALUE_END || bdi[t].value == 0)
		return 0;
	x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + 2 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
	y = pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + 1 + row_no * (pCGI->CFI.separator + pCGI->CFI.block_high)
		+ pCGI->CFI.block_high / 2;
	h = (pCGI->CFI.block_high + 1) / 2;
	if (direction == 0 || direction == 1)
	{
		for (l = 0; l < distance * (pCGI->CFI.block_high + pCGI->CFI.separator); l++)
		{
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			if (pCGI->CBI.block_border)
			{
				cct_gotoxy(x, direction == 0 ? y + pCGI->CFI.block_high / 2: y - pCGI->CFI.block_high / 2);
				cout << setw(pCGI->CFI.block_width) << setfill(' ') << ' ';
			}
			else
			{
				cct_gotoxy(x, y);
				if (pCGI->CFI.separator && (l + 1) % (pCGI->CFI.block_high + 1) == 0)
					for (int i = 0; i < pCGI->CFI.block_width; i += 2)
						cout << pCGI->CFI.h_normal;
				else
					cout << setw(pCGI->CFI.block_width) << setfill(' ') << ' ';
			}
			if (pCGI->CBI.block_border)
			{
				for (int j = y - h + 1; j < y + h; j++)
				{
					cct_gotoxy(x, j);
					cout << setw(pCGI->CFI.block_width) << setfill(' ') << ' ';
				}
				Sleep(pCGI->delay_of_draw_block);
			}
			switch (direction)
			{
			case 0:
				y--;
				break;
			case 1:
				y++;
				break;
			default:
				break;
			}
			Sleep(pCGI->delay_of_block_moved);
			cct_setcolor(bdi[t].bgcolor == -1 ? pCGI->area_bgcolor : bdi[t].bgcolor,
				bdi[t].fgcolor == -1 ? pCGI->area_fgcolor : bdi[t].fgcolor);
			if (pCGI->CBI.block_border)
			{
				for (int j = h - 1; j > 0; j--)
				{
					cct_gotoxy(x, y - j);
					if (j == h - 1)
					{
						cout << pCGI->CBI.top_left;
						for (int i = 0; i < pCGI->CFI.block_width - 4; i += 2)
							cout << pCGI->CBI.h_normal;
						cout << pCGI->CBI.top_right;
					}
					else
					{
						cout << pCGI->CBI.v_normal;
						cout << setw(pCGI->CFI.block_width - 4) << setfill(' ') << ' ';
						cout << pCGI->CBI.v_normal;
					}
				}
				cct_gotoxy(x, y);
				cout << pCGI->CBI.v_normal;
				if (bdi[t].content == NULL)
					cout << setw(pCGI->CFI.block_width - 4) << setiosflags(ios::left) << bdi_value;
				else
					cout << bdi[t].content;
				cct_gotoxy(x + pCGI->CFI.block_width - 2, y);
				cout << pCGI->CBI.v_normal;
				for (int j = 1; j < h; j++)
				{
					cct_gotoxy(x, y + j);
					if (j == h - 1)
					{
						cout << pCGI->CBI.lower_left;
						for (int i = 0; i < pCGI->CFI.block_width - 4; i += 2)
							cout << pCGI->CBI.h_normal;
						cout << pCGI->CBI.lower_right;
					}
					else
					{
						cout << pCGI->CBI.v_normal;
						cout << setw(pCGI->CFI.block_width - 4) << setfill(' ') << ' ';
						cout << pCGI->CBI.v_normal;
					}
				}
				if (pCGI->CBI.block_border && pCGI->CFI.separator)
				{
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					cct_gotoxy(x, direction == 0 ? y + pCGI->CFI.block_high / 2 + 1 : y - pCGI->CFI.block_high / 2 - 1);
					if ((l + 1) % (pCGI->CFI.block_high + 1) == 0)
						for (int i = 0; i < pCGI->CFI.block_width; i += 2)
							cout << pCGI->CFI.h_normal;
				}
				Sleep(pCGI->delay_of_draw_block);
			}
			else
			{
				cct_gotoxy(x, y);
				if (bdi[t].content == NULL)
					cout << setw(pCGI->CFI.block_width - 4) << setiosflags(ios::left) << bdi_value;
				else
					cout << bdi[t].content;
				Sleep(pCGI->delay_of_draw_block);
			}
			Sleep(pCGI->delay_of_block_moved);
		}
	}
	else
	{
		for (l = 2; l <= distance * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator); l += 2)
		{
			cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			if (pCGI->CBI.block_border)
			{
				int u = direction == 2 ? x + pCGI->CFI.block_width - 2 : x;
				for (int i = -h + 1; i < h; i++)
				{
					cct_gotoxy(u, y + i);
					cout << "  ";
				}
			}
			else
				if (pCGI->CFI.separator && l % (pCGI->CFI.block_width + 2) == 0)
					for (int i = -h + 1; i < h; i++)
					{
						cct_gotoxy(x, y + i);
						cout << pCGI->CFI.v_normal;
					}
				else
					for (int i = -h + 1; i < h; i++)
					{
						cct_gotoxy(x, y + i);
						cout << setw(pCGI->CFI.block_width) << ' ';
					}
			if (pCGI->CBI.block_border)
			{
				for (int j = y - h + 1; j < y + h; j++)
				{
					cct_gotoxy(x, j);
					cout << setw(pCGI->CFI.block_width) << setfill(' ') << ' ';
				}
				Sleep(pCGI->delay_of_draw_block);
			}
			switch (direction)
			{
			case 2:
				x -= 2;
				break;
			case 3:
				x += 2;
				break;
			default:
				break;
			}
			Sleep(pCGI->delay_of_block_moved);
			cct_setcolor(bdi[t].bgcolor == -1 ? pCGI->area_bgcolor : bdi[t].bgcolor,
				bdi[t].fgcolor == -1 ? pCGI->area_fgcolor : bdi[t].fgcolor);
			if (pCGI->CBI.block_border)
			{
				for (int j = h - 1; j > 0; j--)
				{
					cct_gotoxy(x, y - j);
					if (j == h - 1)
					{
						cout << pCGI->CBI.top_left;
						for (int i = 0; i < pCGI->CFI.block_width - 4; i += 2)
							cout << pCGI->CBI.h_normal;
						cout << pCGI->CBI.top_right;
					}
					else
					{
						cout << pCGI->CBI.v_normal;
						cout << setw(pCGI->CFI.block_width - 4) << setfill(' ') << ' ';
						cout << pCGI->CBI.v_normal;
					}
				}
				cct_gotoxy(x, y);
				cout << pCGI->CBI.v_normal;
				if (bdi[t].content == NULL)
					cout << setw(pCGI->CFI.block_width - 4) << setiosflags(ios::left) << bdi_value;
				else
					cout << bdi[t].content;
				cct_gotoxy(x + pCGI->CFI.block_width - 2, y);
				cout << pCGI->CBI.v_normal;
				for (int j = 1; j < h; j++)
				{
					cct_gotoxy(x, y + j);
					if (j == h - 1)
					{
						cout << pCGI->CBI.lower_left;
						for (int i = 0; i < pCGI->CFI.block_width - 4; i += 2)
							cout << pCGI->CBI.h_normal;
						cout << pCGI->CBI.lower_right;
					}
					else
					{
						cout << pCGI->CBI.v_normal;
						cout << setw(pCGI->CFI.block_width - 4) << setfill(' ') << ' ';
						cout << pCGI->CBI.v_normal;
					}
				}
				if (pCGI->CBI.block_border && pCGI->CFI.separator)
					if (l % (pCGI->CFI.block_width + 2) == 0)
					{
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						int u = direction == 2 ? x + pCGI->CFI.block_width : x - 2;
						for (int i = -h + 1; i < h; i++)
						{
							cct_gotoxy(u, y + i);
							cout << pCGI->CFI.v_normal;
						}
					}
				Sleep(pCGI->delay_of_draw_block);
			}
			else
			{
				cct_gotoxy(x, y);
				if (bdi[t].content == NULL)
					cout << setw(pCGI->CFI.block_width - 4) << setiosflags(ios::left) << bdi_value;
				else
					cout << bdi[t].content;
				Sleep(pCGI->delay_of_draw_block);
			}
			Sleep(pCGI->delay_of_block_moved);
		}
	}
	cct_setcolor();
	cct_gotoxy(pCGI->extern_left_cols, pCGI->lines - 3);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
		                                               如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
		                          且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值（因为消灭星星的右键标记需要坐标）
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	KeyCode1 = 0;
	KeyCode2 = 0;
	cct_enable_mouse();
	cct_setcursor(3);
	int ret, x, y, action, loop = 1;
	char temp[256];
	while (loop)
	{
		int r = -1, c = -1;
		ret = cct_read_keyboard_and_mouse(x, y, action, KeyCode1, KeyCode2);
		if (ret == CCT_KEYBOARD_EVENT)
		{
			cct_disable_mouse();
			return CCT_KEYBOARD_EVENT;
		}
		else if (ret == CCT_MOUSE_EVENT)
		{
			for (int i = 0; i < pCGI->row_num; i++)
			{
				if (pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + i * pCGI->CFI.block_high + i * pCGI->CFI.separator<y + 1 && pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + (i + 1) * pCGI->CFI.block_high + (i + 1) * pCGI->CFI.separator>y + 1)
				{
					r = i;
					break;
				}
				r = -1;
			}
			for (int j = 0; j < pCGI->col_num; j++)
			{
				if (pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + 2 + j * pCGI->CFI.block_width + j * pCGI->CFI.separator * 2 < x + 1 && pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + 2 + (j + 1) * pCGI->CFI.block_width + (j + 1) * pCGI->CFI.separator * 2 > x + 1)
				{
					c = j;
					break;
				}
				c = -1;
			}
			switch (action)
			{
			case  MOUSE_LEFT_BUTTON_CLICK:
				if (r != -1 && c != -1)
				{
					MAction = action;
					MRow = r;
					MCol = c;
					return CCT_MOUSE_EVENT;
				}
				else
				{
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
					continue;
				}
				break;
			case MOUSE_RIGHT_BUTTON_CLICK:
				MAction = action;
				MRow = r == -1 ? MRow : -1;
				MCol = c == -1 ? MCol : -1;
				return CCT_MOUSE_EVENT;
				break;
			case MOUSE_ONLY_MOVED:
				break;
			}
			if (r == -1 || c == -1)
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
			else if(r != MRow || c != MCol)
			{
				MAction = MOUSE_ONLY_MOVED;
				MRow = r;
				MCol = c;
				sprintf(temp, "[当前光标] %c行%d列", char(MRow + 'A'), MCol);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return CCT_MOUSE_EVENT;
			}
		}
	}
	return 0; //此句可根据需要修改
}