/* 2153592 ���ݷ� ��01 */
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
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */


/* --------------------------------------------------
		�˴����Ը�����Ҫ���ڲ��������ߺ���
		1�����������ޣ�����Ϊ gmw_inner_* 
		2����������
		3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
   -------------------------------------------------- */


/* ----------------------------------------------- 
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
            2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
				    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if (cascade)
	{
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;                         //��Ϸ���������
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;            //״̬����Ϣ		
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;            //״̬����Ŀ
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
            2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	if (!strcmp(fontname, "Terminal") || !strcmp(fontname, "������"))
		strcpy(pCGI->CFT.font_type, fontname);
	else
		return -1;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1; 
	/*	�X �T �h �T �[
    	�U          �U
	    �c �T �� �T �f
	    �U          �U
	    �^ �T �k �T �a */
	char tabs[4][11][3] =
	{ {"�X","�^","�[","�a","�T","�U","�j","�m","�d","�g","�p"},
	  {"��","��","��","��","��","��","��","��","��","��","��"},
	  {"�V","�\","�Y","�_","�T","��","�h","�k","�b","�e","�n"},
	  {"�W","�]","�Z","�`","��","�U","�i","�l","�c","�f","�o"} };
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal, const char* h_top_separator,
	const char* h_lower_separator, const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFI.bgcolor = bgcolor == -1 ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = fgcolor == -1 ? pCGI->area_fgcolor : fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1; 
    /*  �X �T �[
    	�U    �U
    	�^ �T �a */
	char tabs[4][6][3] =
	{ {"�X","�^","�[","�a","�T","�U"},
	  {"��","��","��","��","��","��"},
	  {"�V","�\","�Y","�_","�T","��"},
	  {"�W","�]","�Z","�`","��","�U"} };
	strcpy(pCGI->CBI.top_left, tabs[type - 1][0]);
	strcpy(pCGI->CBI.lower_left, tabs[type - 1][1]);
	strcpy(pCGI->CBI.top_right, tabs[type - 1][2]);
	strcpy(pCGI->CBI.lower_right, tabs[type - 1][3]);
	strcpy(pCGI->CBI.h_normal, tabs[type - 1][4]);
	strcpy(pCGI->CBI.v_normal, tabs[type - 1][5]);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	strcpy(pCGI->CBI.top_left, top_left);
	strcpy(pCGI->CBI.lower_left, lower_left);
	strcpy(pCGI->CBI.top_right, top_right);
	strcpy(pCGI->CBI.lower_right, lower_right);
	strcpy(pCGI->CBI.h_normal, h_normal);
	strcpy(pCGI->CBI.v_normal, v_normal);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_row_no = on_off;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;
	pCGI->SLI.width = pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_col_no = on_off;	
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line
		+ pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->draw_frame_with_col_no;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* �����ñ�� */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const char* msg, const char* catchy_msg)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
		                                               ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
		                          �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ����Ϊ�������ǵ��Ҽ������Ҫ���꣩
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
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
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
			else if(r != MRow || c != MCol)
			{
				MAction = MOUSE_ONLY_MOVED;
				MRow = r;
				MCol = c;
				sprintf(temp, "[��ǰ���] %c��%d��", char(MRow + 'A'), MCol);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				return CCT_MOUSE_EVENT;
			}
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}