/* 2153592 胡逸凡 信01 */
#include<iostream>
#include<windows.h>
#include<math.h>
#include<conio.h>
#include"../include/cmd_hdc_tools.h"
using namespace std;

#define YELLOW 0
#define BLUE 1
#define BLACK 2
#define randrom_color 3      //颜色风格选择
#define LEFT -1
#define RIGHT 1
#define UP 1
#define DOWN -1
#define PI  3.14159
#define base_x 600
#define base_y 700           //基准点

/***************************************************************************
曲线为n条线段连接而成，每条线段两端点坐标为(x1i,y1i),(x2i,y2i)，其中(1<=i<=n)
x1i = 1.5 * sin(i * PI / 250 + PI / 3) ^ 7
y1i = -cos(3 * i * PI / 250) ^ 2 / 4
x2i = sin(3 * i * PI / 250 + PI / 5) / 5
y2i = 1.5 * sin(i * PI / 250 - PI / 3) ^ 2
***************************************************************************/

/***************************************************************************
  函数名称：swallow
  功    能：画燕子曲线
  输入参数：int x              ：起始点（嘴）的横坐标
            int y              ：起始点（嘴）的纵坐标
            int figure         ：燕子的大小
			int begin          ：循环的开始点
			int end            ：循环的结束点
			int style          ：颜色风格
			int left_or_right  ：一次画半只燕子，决定左右
			int up_or_down     ：燕子的朝向，向上或向下
			int angles         ：以起始点为圆心的旋转角度，顺时针为正
  返 回 值：无
  说    明：循环的起始点不同决定画的燕子的部位：0-100 翅膀；
            100-150 省略掉，避免凌乱；150-175 腹部；175-185 嘴；185-225
			起始点不为0表示扇动翅膀，颜色也稍有变化
***************************************************************************/
static void swallow(int x, int y, int figure, int begin, int end, int style, int left_or_right, int up_or_down = UP, int angles = 0)
{
	int i = 1, x1, y1, x2, y2, f1, f2, f3, f4;
	srand((unsigned)time(NULL));
	for (i = begin; i <= end; i++)
	{
		if (style == YELLOW)
			if (i <= 100)
				if (begin)
					hdc_set_pencolor(255, 205 + i / 2, 0);
				else
					hdc_set_pencolor(255 - i / 2, 205 + i / 2, 0);
			else if (i <= 175)
				hdc_set_pencolor(178 + (i - 150) / 5, 34 + (i - 150) / 5, 34 + (i - 150) / 5);
			else if (i <= 185)
				hdc_set_pencolor(255, 165, 0);
			else if (i < 225)
				hdc_set_pencolor(139, 162, 206);
			else
				if (begin)
					hdc_set_pencolor(238, 201, 0);
				else
					hdc_set_pencolor(205, 149, 12);
		else if (style == BLUE)
			if (i <= 100)
				if (begin)
					hdc_set_pencolor(30, 150 - i / 2, 255 - i / 2);
				else
					hdc_set_pencolor(30, 150 - i / 2, 255);
			else if (i <= 175)
				hdc_set_pencolor(178 + (i - 150) / 5, 34 + (i - 150) / 5, 34 + (i - 150) / 5);
			else if (i <= 185)
				hdc_set_pencolor(255, 165, 0);
			else if (i < 225)
				hdc_set_pencolor(139, 162, 206);
			else
				if (begin)
					hdc_set_pencolor(0, 0, 205);
				else
					hdc_set_pencolor(65, 105, 225);
		else if (style == randrom_color)
			hdc_set_pencolor(rand() % 255, rand() % 255, rand() % 255);
		else
			hdc_set_pencolor(0, 0, 0);
		f1 = left_or_right * int(3 * figure * sin(i * PI / 250 + PI / 3) * sin(i * PI / 250 + PI / 3)
			* sin(i * PI / 250 + PI / 3) * sin(i * PI / 250 + PI / 3) * sin(i * PI / 250 + PI / 3)
			* sin(i * PI / 250 + PI / 3) * sin(i * PI / 250 + PI / 3) * sin(i * PI / 250 + PI / 3) / 2);
		f2 = left_or_right * int(figure * sin(3 * i * PI / 250 + PI / 5) / 5);
		f3 = -up_or_down * int(figure * cos(3 * i * PI / 250) * cos(3 * i * PI / 250) / 4);
		f4 = up_or_down * int(3 * figure * sin(i * PI / 250 - PI / 3) * sin(i * PI / 250 - PI / 3) / 2);
		x1 = x + int(f1 * cos(angles * PI / 180) - f3 * sin(angles * PI / 180));
		x2 = x + int(f2 * cos(angles * PI / 180) - f4 * sin(angles * PI / 180));
		y1 = y + int(f3 * cos(angles * PI / 180) + f1 * sin(angles * PI / 180));
		y2 = y + int(f4 * cos(angles * PI / 180) + f2 * sin(angles * PI / 180));
		hdc_line(x1, y1, x2, y2);
			if (i == 100)
				i = 150;
		if (style == randrom_color)
			Sleep(1);
	}
}

/***************************************************************************
  函数名称：roration
  功    能：旋转，燕子盘旋
  输入参数：int x               ：起始点（嘴）的横坐标
            int y               ：起始点（嘴）的纵坐标
            int figure          ：燕子的大小
			int style           ：颜色风格
			int rotation_angles ：旋转的角度
  返 回 值：无
  说    明：旋转角度顺时针为正
***************************************************************************/
static void roration(int x, int y, int figure, int style, int up_or_down, int rotation_angles)
{
	int angles;
	for (angles = 18; angles <= rotation_angles; angles += 18)
	{
		hdc_cls();
		cout << "盘旋";
		if ((angles / 18) % 2)
		{
			swallow(x, y, figure, 30, 250, style, RIGHT, up_or_down, angles);
			swallow(x, y, figure, 30, 250, style, LEFT, up_or_down, angles);
		}
		else
		{
			swallow(x, y, figure, 1, 285, style, RIGHT, up_or_down, angles);
			swallow(x, y, figure, 1, 285, style, LEFT, up_or_down, angles);
		}
		Sleep(200);
	}
}

/***************************************************************************
  函数名称：fly
  功    能：燕子飞行
  输入参数：int x          ：起始点（嘴）的横坐标
			int y          ：起始点（嘴）的纵坐标
			int figure     ：燕子的大小
			int style      ：颜色风格
			int steps      ：移动步数
			int move       ：步幅长度
  返 回 值：无
  说    明：以现在步数、步幅为优，否则可能超出显示区
***************************************************************************/
static void fly(int x, int y, int figure, int style, int steps, int move)
{
	int j = 1;
	bool is_up = 1;
	while (j != 0)
	{
		hdc_cls();
		cout << "飞行";
		if (j % 2)
		{
			swallow(x, y - j * move, figure, 30, 250, style, RIGHT, is_up ? UP : DOWN);
			swallow(x, y - j * move, figure, 30, 250, style, LEFT, is_up ? UP : DOWN);
		}
		else
		{
			swallow(x, y - j * move, figure, 1, 285, style, RIGHT, is_up ? UP : DOWN);
			swallow(x, y - j * move, figure, 1, 285, style, LEFT, is_up ? UP : DOWN);
		}                                                                                   //向前/后飞行
		Sleep(200);
		if (is_up)
			j++;
		else
			j--;
		if (j == steps)
		{
			is_up = 0;
			j--;
			roration(x, y - j * move, figure, style, UP, 180);
		}
	}
}

/***************************************************************************
  函数名称：init
  功    能：初始化并错误处理
  输入参数：int& style  ：图案风格（黄色、蓝色、黑色、彩色）
            int& choice ：输入玩家的选择
  返 回 值：无
  说    明：
***************************************************************************/
static void init(int& style, char& choice)
{
	hdc_cls();
	cout << "燕子" << endl;
	cout << "请选择风格(Y-黄色,B-蓝色,L-黑色):";
	while (1)
	{
		choice = _getch();
		if (choice == 'Y' || choice == 'y' || choice == 'B' || choice == 'b' || choice == 'L' || choice == 'l')
		{
			cout << choice;
			break;
		}
	}
	if (choice == 'Y' || choice == 'y')
		style = YELLOW;
	else if (choice == 'B' || choice == 'b')
		style = BLUE;
	else
		style = BLACK;
}

void hdc_draw_Mathematical_curve_2153592()
{
	int figure = 250, style, x = base_x, y = base_y;
	char choice;
	init(style, choice);
	swallow(x, y, figure, 1, 285, style, RIGHT);
	swallow(x, y, figure, 1, 285, style, LEFT);
	Sleep(1500);
	hdc_set_delay(0);                                                                       //取消延时，体现移动
	fly(x, y, figure, style, 5, 80);                                                        //飞行
	roration(x, y, figure, style, DOWN, 180);                                               //掉头-180    盘旋-180*n 可以自行改变
	Sleep(500);
	swallow(x, y, figure, 1, 450, randrom_color, RIGHT);
	swallow(x, y, figure, 1, 450, randrom_color, LEFT);                                     //彩色
}