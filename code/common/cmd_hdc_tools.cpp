/* 2153592 ���ݷ� ��01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <math.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//δ���������²Ŵ���������

/* ���������Ҫ��ͷ�ļ� */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS�д˴����в����ߣ���Ӱ������ʹ��

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */

static int _max(int a,int b)
{
	return a > b ? a : b;
}

static int _min(int a, int b)
{
	return a < b ? a : b;
}

static int cross_product(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

static bool is_in_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
	if (cross_product(x1, y1, x2, y2, x3, y3) < 0)
		return is_in_triangle(x1, y1, x3, y3, x2, y2, x, y);
	if (cross_product(x1, y1, x2, y2, x, y) >= 0 &&
		cross_product(x2, y2, x3, y3, x, y) >= 0 &&
		cross_product(x3, y3, x1, y1, x, y) >= 0)
		return 1;
	else
		return 0;
}

int change_angle(int direction, const int rotation_angles)   //˳ʱ��directionΪ1����ʱ��Ϊ-1
{
	return direction * rotation_angles;
}

/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
			const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("��������", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cct_cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
	/* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x1,y1)-(x2,y2)֮�仭��һ�����ص������
  ���������const int x1�����x���꣬���Ͻ�Ϊ(0,0)
			const int y1�����y���꣬���Ͻ�Ϊ(0,0)
			const int x2���յ�y���꣬���Ͻ�Ϊ(0,0)
			const int y2���յ�y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
			const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_base_point/hdc_base_line���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ����߶�
	 ���������const int x1		������x
			   const int y1		������y
			   const int x2		���յ��x
			   const int y2		���յ��y
			   const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			   const int RGB_value	���߶ε���ɫ����ȱʡֵ��
	 �� �� ֵ��
	 ˵    ���������������û���hdc_point����ϣ��ٶ�����
					 Ҳ������hdc_base_point/hdc_base_line����ϣ��ٶȿ죩
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	//���Ӷ˵�
	int tn = thickness;
	if (tn < 1)
		tn = 1;
	const int tn_end = (tn <= 1) ? 1 : tn / 2;
	int angle, level, _x1, _x2, _y1, _y2;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, old_x2 = INT_MAX, old_y2 = INT_MAX;
	for (level = 1; level <= tn_end; level++)
		for (angle = 0; angle <= 360; angle++) 
		{
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			_x1 = x1 + (int)(level * sin(angle * PI / 180));
			_y1 = y1 - (int)(level * cos(angle * PI / 180));
			_x2 = x2 + (int)(level * sin(angle * PI / 180));
			_y2 = y2 - (int)(level * cos(angle * PI / 180));
			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (_x1 != old_x1 || _y1 != old_y1 || _x2 != old_x2 || _y2 != old_y2)
			{
				old_x1 = _x1;
				old_y1 = _y1;
				old_x2 = _x2;
				old_y2 = _y2;
				hdc_base_line(_x1, _y1, _x2, _y2);
			}
		}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
			const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	srand((unsigned int)time(NULL));
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	if (filled)
	{
		int min_x, min_y, max_x, max_y, x, y;
		min_x = _min(x1, _min(x2, x3));
		min_y = _min(y1, _min(y2, y3));
		max_x = _max(x1, _max(x2, x3));
		max_y = _max(y1, _max(y2, y3));
		for (x = min_x; x <= max_x; x++)
			for (y = min_y; y <= max_y; y++)
				if (is_in_triangle(x1, y1, x2, y2, x3, y3, x, y))
					hdc_point(x, y, thickness, RGB_value);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{

	int x1, y1, x2, y2, x3, y3, x4, y4, angles;
	angles = change_angle(1, rotation_angles);
	x1 = left_up_x;
	y1 = left_up_y;
	x2 = x1 + int(width * cos(angles * PI / 180));
	y2 = y1 + int(width * sin(angles * PI / 180));
	x3 = x2 - int(high * sin(angles * PI / 180));
	y3 = y2 + int(high * cos(angles * PI / 180));
	x4 = x1 - int(high * sin(angles * PI / 180));
	y4 = y1 + int(high * cos(angles * PI / 180));
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x4, y4, thickness, RGB_value);
	hdc_line(x4, y4, x1, y1, thickness, RGB_value);
	if (filled)
	{
		if (RGB_value != INVALID_RGB)
			hdc_set_pencolor(RGB_value);
		int min_x, min_y, max_x, max_y, x, y;
		min_x = _min(_min(x1, x4), _min(x2, x3));
		min_y = _min(_min(y1, y4), _min(y2, y3));
		max_x = _max(_max(x1, x4), _max(x2, x3));
		max_y = _max(_max(y1, y4), _max(y2, y3));
		for (x = min_x; x <= max_x; x++)
			for (y = min_y; y <= max_y; y++)
				if (is_in_triangle(x1, y1, x2, y2, x3, y3, x, y) || is_in_triangle(x1, y1, x4, y4, x3, y3, x, y))
					hdc_base_point(x, y);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int begin, end, angle;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	if (angle_begin < 0 && angle_begin % 360 == 0)
		begin = 0;
	else
		begin = angle_begin;
	if (fabs(angle_end - begin) >= 360)
	{
		begin = 0;
		end = 360;
	}
	else
	{
		begin = angle_begin < 0 ? 360 + angle_begin : angle_begin;
		end = angle_end < 0 ? 360 + angle_end : angle_end;
		begin -= begin / 360 * 360;
		end -= end / 360 * 360;
		if (!end)
			end = 360;
		if (begin > end)
			begin -= 360;
	}
	for (angle = begin; angle < end; angle++)
		hdc_line(point_x + int(radius * sin(angle * PI / 180)), point_y - int(radius * cos(angle * PI / 180)),
			point_x + int(radius * sin((angle + 1) * PI / 180)), point_y - int(radius * cos((angle + 1) * PI / 180)), thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int x1, y1, x2, y2, begin, end;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	if (angle_begin < 0 && angle_begin % 360 == 0)
		begin = 0;
	else
		begin = angle_begin;
	if (fabs(angle_end - begin) >= 360)
	{
		begin = 0;
		end = 360;
	}
	else
	{
		begin = angle_begin < 0 ? 360 + angle_begin : angle_begin;
		end = angle_end < 0 ? 360 + angle_end : angle_end;
		begin -= begin / 360 * 360;
		end -= end / 360 * 360;
		if (!end)
			end = 360;
		if (begin > end)
			begin -= 360;
	}
	x1 = point_x + int(radius * sin(begin * PI / 180));
	y1 = point_y - int(radius * cos(begin * PI / 180));
	x2 = point_x + int(radius * sin(end * PI / 180));
	y2 = point_y - int(radius * cos(end * PI / 180));
	if ((end - begin) % 360)
	{
		hdc_line(point_x, point_y, x1, y1, thickness, RGB_value);
		hdc_line(point_x, point_y, x2, y2, thickness, RGB_value);
	}
	hdc_arc(point_x, point_y, radius, begin, end, thickness, RGB_value);
	if (filled)
	{
		int x, y;
		int an, l;
		begin *= 100;
		end *= 100;                                                                                 //�Ƕ�ͳһ����100����Сarcsin�����
		for (y = point_y - radius + 1; y < point_y + radius; y++)
			for (x = point_x - radius + 1; x < point_x + radius; x++)
			{
				l = (x - point_x) * (x - point_x) + (y - point_y) * (y - point_y);
				if (l <= radius * radius)
				{
					if (l)
					{
						if (y - point_y >= 0)
							an = 18000 - int(asin((x - point_x) / sqrt(l)) / PI * 18000);
						else
							an = int(asin((x - point_x) / sqrt(l)) / PI * 18000);
						if (an >= (begin >= 0 ? begin : begin + 36000) && an <= (begin >= 0 ? end : end + 36000))
							hdc_base_point(x, y);
						if (begin >= 0)
							an = (an < 0 ? an + 36000 : an);
						if (an >= begin && an <= end)
							hdc_base_point(x, y);
					}
					else
						hdc_base_point(x, y);
				}
			}
		/*for (an = begin; an < end; an++)
			for (l = radius - 1; l >= 0; l--)
			{
				hdc_line(point_x + int(l * sin(an * PI / 180)), point_y - int(l * cos(an * PI / 180)),
					point_x + int(l * sin((an + 1) * PI / 180)), point_y - int(l * cos((an + 1) * PI / 180)), thickness, RGB_value);
			}*/
			
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 360, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	int x = 0, y = radius_b, x1, y1, x2, y2, angles;
	double d = radius_b * radius_b + radius_a * radius_a * (0.25 - radius_b);
	angles = change_angle(1, rotation_angles);
	while (y >= 0)
	{
		x1 = int(x * cos(angles * PI / 180) - y * sin(angles * PI / 180));
		y1 = int(y * cos(angles * PI / 180) + x * sin(angles * PI / 180));
		x2 = int(y * sin(angles * PI / 180) + x * cos(angles * PI / 180));
		y2 = int(x * sin(angles * PI / 180) - y * cos(angles * PI / 180));
		hdc_point(point_x + x1, point_y + y1, thickness, RGB_value);
		hdc_point(point_x + x2, point_y + y2, thickness, RGB_value);
		hdc_point(point_x - x2, point_y - y2, thickness, RGB_value);
		hdc_point(point_x - x1, point_y - y1, thickness, RGB_value);
		if (filled)
		{
			hdc_line(point_x + x1, point_y + y1, point_x - x1, point_y - y1, thickness, RGB_value);
			hdc_line(point_x + x1, point_y + y1, point_x - x2, point_y - y2, thickness, RGB_value);
			hdc_line(point_x + x1, point_y + y1, point_x + x2, point_y + y2, thickness, RGB_value);
			hdc_line(point_x + x2, point_y + y2, point_x - x2, point_y - y2, thickness, RGB_value);
			hdc_line(point_x + x2, point_y + y2, point_x - x1, point_y - y1, thickness, RGB_value);
			hdc_line(point_x - x1, point_y - y1, point_x - x2, point_y - y2, thickness, RGB_value);
		}
		if (radius_a * radius_a * (y - 0.5) > radius_b * radius_b * (x + 1))
			if (d <= 0)
			{
				d += radius_b * radius_b * (2 * x + 3);
				x++;
			}
			else
			{
				d += radius_b * radius_b * (2 * x + 3) + radius_a * radius_a * (2 - 2 * y);
				x++;
				y--;
			}
		else
			if (d <= 0)
			{
				d += radius_b * radius_b * (2 * x + 2) + radius_a * radius_a * (3 - 2 * y);
				x++;
				y--;
			}
			else
			{
				d += radius_a * radius_a * (3 - 2 * y);
				y--;
			}
	}
}

#endif !HDC_SERIES_BY_TEACHER