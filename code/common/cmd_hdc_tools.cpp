/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <math.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */

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

int change_angle(int direction, const int rotation_angles)   //顺时针direction为1，逆时针为-1
{
	return direction * rotation_angles;
}

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
			const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
	/* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
			const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

   /***************************************************************************
	 函数名称：
	 功    能：画线段
	 输入参数：const int x1		：起点的x
			   const int y1		：起点的y
			   const int x2		：终点的x
			   const int y2		：终点的y
			   const int thickness	：线段的粗细（有缺省值）
			   const int RGB_value	：线段的颜色（有缺省值）
	 返 回 值：
	 说    明：本函数可以用基于hdc_point的组合（速度慢）
					 也可以用hdc_base_point/hdc_base_line的组合（速度快）
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	//连接端点
	int tn = thickness;
	if (tn < 1)
		tn = 1;
	const int tn_end = (tn <= 1) ? 1 : tn / 2;
	int angle, level, _x1, _x2, _y1, _y2;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, old_x2 = INT_MAX, old_y2 = INT_MAX;
	for (level = 1; level <= tn_end; level++)
		for (angle = 0; angle <= 360; angle++) 
		{
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			_x1 = x1 + (int)(level * sin(angle * PI / 180));
			_y1 = y1 - (int)(level * cos(angle * PI / 180));
			_x2 = x2 + (int)(level * sin(angle * PI / 180));
			_y2 = y2 - (int)(level * cos(angle * PI / 180));
			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
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
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
			const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
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
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：给出左上角坐标及边长，画出一个正方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
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
		end *= 100;                                                                                 //角度统一扩大100倍较小arcsin的误差
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
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 360, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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