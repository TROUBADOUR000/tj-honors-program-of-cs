/* 2153592 胡逸凡 信01 */
#include<iostream>
#include<windows.h>
#include<math.h>
#include"../include/cmd_hdc_tools.h"
using namespace std;

#define PI  3.14159

/* background color & figure */
static const int BACKGROUND_WIDTH = 900;
static const int BACKGROUND_HEIGHT = 700;
static const int BACKGROUND_COLOR = RGB(255, 255, 255);

/* side boundary */
static const int SIDE_COLOR = RGB(0, 0, 0);

/* head */
static const int HEAD_A = 130;
static const int HEAD_B = 100;
static const int HEAD_RA = 20;
static const int HEAD_RB = 15;
static const int HEAD_COLOR = RGB(5, 35, 235);
static const int FACE_COLOR = RGB(245, 240, 45);
static const int CHEEK_COLOR = RGB(245, 120, 120);


/* eye */
static const int EYE_A = 38;
static const int EYE_B = 30;
static const int EYE_R = 25;
static const int BALL_R = 15;
static const int interorbital_dis = 110;                  //眼间距
static const int EYE_COLOR = RGB(255, 255, 255);
static const int BALL_COLOR = RGB(139, 90, 43);

/* hair */
static const int HAIR_COLOR = RGB(0, 0, 0);

/* hand */
static const int HAND_COLOR = RGB(255, 0, 0);

/* flag */
static const int FLAG_WIDTH_TOP = 25;
static const int FLAG_HEIGHT = 50;
static const int FLAG_LENGYH = 12;
static const int FLAG_COLOR = RGB(255, 255, 255);

/* clothes */
static const int CLOTHES_COLOR = RGB(5, 35, 235);
static const int BUTTON_COLOR = RGB(120, 110, 110);

/* shield */
static const int SHIELD_COLOR1 = RGB(255, 0, 0);
static const int SHIELD_COLOR2 = RGB(255, 255, 235);
static const int SHIELD_COLOR3 = RGB(5, 35, 235);

/* another */
static const int JEANS_COLOR = RGB(108, 166, 222);
static const int GLASS_COLOR = RGB(202, 202, 202);
static const int BOARD_COLOR = RGB(139, 69, 0);

//背景
static void background(const int base_x, const int base_y)
{
    hdc_rectangle(80, 20, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, 0, 1, 2, BACKGROUND_COLOR);
    hdc_line(80, base_y + 204, 80 + BACKGROUND_WIDTH, base_y + 204, 3, SIDE_COLOR);
}

//头发
static void draw_hair(const int base_x, const int base_y)
{
    int x1, y1, x2, y2, x3, y3;
    /* hair */
    x1 = base_x;
    y1 = base_y - 2 * HEAD_B + 3;
    x2 = base_x - 5;
    y2 = y1 - 30;
    x3 = base_x + 5;
    y3 = y1 - 30;
    hdc_triangle(x1, y1, x2, y2, x3, y3, 1, 6, HAIR_COLOR);
    x2 = base_x - 20;
    y2 = y1 - 26;
    x3 = base_x - 15;
    y3 = y1 - 25;
    hdc_triangle(x1 - 15, y1, x2, y2, x3, y3, 1, 6, HAIR_COLOR);
    x2 = base_x + 20;
    y2 = y1 - 29;
    x3 = base_x + 15;
    y3 = y1 - 27;
    hdc_triangle(x1 + 15, y1, x2, y2, x3, y3, 1, 6, HAIR_COLOR);
}

//身体、脑袋、脸颊 有较多覆盖与填充因此放在一个函数
static void draw_head_face_cheek_body(const int base_x, const int base_y)
{
    int ey, r;
    ey = base_y - HEAD_B;
    /* face */
    hdc_ellipse(base_x, ey + HEAD_B / 5, HEAD_A + 5, HEAD_B + 5, 0, 1, 3, FACE_COLOR);
    /* body */
    hdc_ellipse(base_x, ey + HEAD_B / 5 + 200, HEAD_A + 5, 55, 0, 1, 3, CLOTHES_COLOR);
    hdc_rectangle(base_x - HEAD_A - 5, ey + HEAD_B / 5, 2 * (HEAD_A + 4), 200, 0, 1, 3, FACE_COLOR);
    /* head */
    hdc_ellipse(base_x, ey, HEAD_A, HEAD_B, 0, 1, 3, HEAD_COLOR);
    hdc_ellipse(base_x, ey, HEAD_A, HEAD_B, 0, 0, 3, SIDE_COLOR);
    r = (HEAD_B + 5) * (HEAD_B + 5) / (HEAD_A + 5);                                 //曲率半径计算
    hdc_circle(base_x + HEAD_A - r + 2, ey + HEAD_B / 5, r, 1, 3, HEAD_COLOR);
    hdc_circle(base_x - HEAD_A + r - 4, ey + HEAD_B / 5, r, 1, 3, HEAD_COLOR);
    /* cheek */
    hdc_ellipse(base_x - HEAD_A / 2, base_y - HEAD_B / 3 - 2, 51 * HEAD_RA / 20, 2 * HEAD_RB, 0, 1, 3, FACE_COLOR);
    hdc_ellipse(base_x + HEAD_A / 2, base_y - HEAD_B / 3 - 2, 51 * HEAD_RA / 20, 2 * HEAD_RB, 0, 1, 3, FACE_COLOR);      //特殊处理为了不挡住轮廓线
    hdc_ellipse(base_x, base_y - HEAD_B / 3, HEAD_RA, HEAD_RB / 2, 0, 1, 3, FACE_COLOR);
    hdc_ellipse(base_x, base_y - HEAD_B / 6, 5 * HEAD_RA, 3 * HEAD_RB / 2, 0, 1, 3, FACE_COLOR);
    hdc_ellipse(base_x - HEAD_A / 2, base_y - HEAD_B / 2, 3 * HEAD_RA, 3 * HEAD_RB / 2, 0, 1, 4, HEAD_COLOR);
    hdc_ellipse(base_x + HEAD_A / 2, base_y - HEAD_B / 2, 3 * HEAD_RA, 3 * HEAD_RB / 2, 0, 1, 4, HEAD_COLOR);
    hdc_ellipse(base_x - HEAD_A + HEAD_RA + 15, int(ey + HEAD_B / 2) + 20, HEAD_RA, HEAD_RB, 0, 1, 3, CHEEK_COLOR);
    hdc_ellipse(base_x + HEAD_A - HEAD_RA - 15, int(ey + HEAD_B / 2) + 20, HEAD_RA, HEAD_RB, 0, 1, 3, CHEEK_COLOR);
}

//嘴
static void draw_mouth(const int base_x, const int base_y)
{
    hdc_ellipse(base_x, base_y - 5, 12, 8, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x - 14, base_y - 5, 28, 9, 0, 1, 3, FACE_COLOR);

}

//眼睛
static void draw_eyes(const int base_x, const int base_y)
{
    int ey = base_y - HEAD_B;
    hdc_ellipse(base_x + interorbital_dis / 2, ey, EYE_A, EYE_B, 13, 1, 3, FACE_COLOR);
    hdc_ellipse(base_x + interorbital_dis / 2, ey, EYE_A, EYE_B, 13, 0, 3, SIDE_COLOR);
    hdc_ellipse(base_x - interorbital_dis / 2, ey, EYE_A, EYE_B, -13, 1, 3, FACE_COLOR);
    hdc_ellipse(base_x - interorbital_dis / 2, ey, EYE_A, EYE_B, -13, 0, 3, SIDE_COLOR);
    hdc_circle(base_x + interorbital_dis / 2, ey, EYE_R, 1, 3, EYE_COLOR);
    hdc_circle(base_x + interorbital_dis / 2, ey, EYE_R, 0, 3, SIDE_COLOR);
    hdc_circle(base_x - interorbital_dis / 2, ey, EYE_R, 1, 3, EYE_COLOR);
    hdc_circle(base_x - interorbital_dis / 2, ey, EYE_R, 0, 3, SIDE_COLOR);
    /* 眼球 */
    hdc_circle(base_x + interorbital_dis / 2, ey, BALL_R, 1, 3, BALL_COLOR);
    hdc_circle(base_x - interorbital_dis / 2, ey, BALL_R, 1, 3, BALL_COLOR);
    hdc_ellipse(base_x + interorbital_dis / 2 - 7, ey - 4, 3, 4, 5, 1, 3, EYE_COLOR);
    hdc_ellipse(base_x - interorbital_dis / 2 - 7, ey - 4, 3, 4, 5, 1, 3, EYE_COLOR);
    hdc_circle(base_x + interorbital_dis / 2, ey, BALL_R/3, 1, 3, SIDE_COLOR);
    hdc_circle(base_x - interorbital_dis / 2, ey, BALL_R/3, 1, 3, SIDE_COLOR);
}

//美队'A'
static void draw_flag(const int base_x, const int base_y)
{
    int x1, x2, y, i, y1;
    x1 = base_x - FLAG_WIDTH_TOP / 2;
    x2 = base_x + FLAG_WIDTH_TOP / 2;
    y = base_y - HEAD_B - EYE_B - FLAG_HEIGHT;
    hdc_line(x1, y, x2, y, 4, SIDE_COLOR);
    i = 0;
    while (i < 4)
    {
        i++;
        y1 = y;
        while (1)
        {
            if (y % 2)
            {
                x1--;
                x2++;
            }
            y++;
            hdc_line(x1, y, x2, y, 4, SIDE_COLOR);
            hdc_line(x1 + 1, y, x2 - 1, y, 4, FLAG_COLOR);
            if (y - y1 == 6)
                break;
        }
        i++;
        y1 = y;
        while (1)
        {
            if (y % 2)
            {
                x1--;
                x2++;
            }
            y++;
            hdc_line(x1, y, x1 + FLAG_LENGYH, y, 4, SIDE_COLOR);
            hdc_line(x1 + 1, y, x1 + FLAG_LENGYH - 1, y, 4, FLAG_COLOR);
            hdc_line(x2 - FLAG_LENGYH, y, x2, y, 4, SIDE_COLOR);
            hdc_line(x2 - FLAG_LENGYH + 1, y, x2 - 1, y, 4, FLAG_COLOR);
            if (y - y1 == (i == 2 ? 12 : 10))
                break;
        }
    }
    hdc_line(x1, y, x1 + FLAG_LENGYH, y, 4, SIDE_COLOR);
    hdc_line(x2 - FLAG_LENGYH, y, x2, y, 4, SIDE_COLOR);
}

//美队星星标志
static void draw_star(const int _x, const int _y, int r)
{
    int t = _y + r;
    int x[8] = { 0,int(r * cos(PI / 10)),int(r * cos(3 * PI / 10)),-int(r * cos(3 * PI / 10)),
        -int(r * cos(PI / 10)),int(r * cos(PI / 10) * 0.382),0,-int(r * cos(PI / 10) * 0.382) };
    int y[8] = { r,int(r * sin(PI / 10)),-int(r * sin(3 * PI / 10)),-int(r * sin(3 * PI / 10)),
        int(r * sin(PI / 10)),-int(r * sin(PI / 10) * 0.382),-int(r * 0.382),-int(r * sin(PI / 10) * 0.382) };
    hdc_triangle(_x + x[0], t - y[0], _x + x[3], t - y[3], _x + x[5], t - y[5], 1, 3, FLAG_COLOR);
    hdc_triangle(_x + x[0], t - y[0], _x + x[2], t - y[2], _x + x[7], t - y[7], 1, 3, FLAG_COLOR);
    hdc_triangle(_x + x[1], t - y[1], _x + x[4], t - y[4], _x + x[6], t - y[6], 1, 3, FLAG_COLOR);
}

//小黄人穿的美队的衣服
static void draw_clothes(const int base_x, const int base_y)
{
    hdc_rectangle(base_x - 100, base_y + 20, 200, 100, 0, 1, 3, CLOTHES_COLOR);
    hdc_rectangle(base_x - 90, base_y + 33, 59, 10, -150, 1, 3, CLOTHES_COLOR);
    hdc_rectangle(base_x + 90, base_y + 26, 52, 10, -30, 1, 3, CLOTHES_COLOR);
    /* button */
    hdc_circle(base_x - 90, base_y + 30, 4, 1, 3, BUTTON_COLOR);
    hdc_circle(base_x + 90, base_y + 30, 4, 1, 3, BUTTON_COLOR);
    /* star */
    draw_star(base_x, base_y + 30, 40);
}

//胳膊和手
static void draw_arm(const int base_x, const int base_y)
{
    /* right arm */
    hdc_rectangle(base_x + 132, base_y + 35, 52, 12, 30, 1, 4, FACE_COLOR);
    hdc_rectangle(base_x + 166, base_y + 55, 25, 12, -60, 1, 4, FACE_COLOR);
    /* right hand */
    hdc_ellipse(base_x + 198, base_y + 15, 20, 8, -60, 1, 3, HAND_COLOR);
    hdc_rectangle(base_x + 180, base_y + 30, 30, 12, -60, 1, 3, HAND_COLOR);
    hdc_rectangle(base_x + 176, base_y + 28, 20, 10, 30, 1, 3, HAND_COLOR);
    hdc_ellipse(base_x + 190, base_y + 14, 12, 6, -145, 1, 3, HAND_COLOR);
    hdc_ellipse(base_x + 205, base_y + 16, 14, 6, -10, 1, 3, HAND_COLOR);
}

//腿和鞋子
static void draw_legs(const int base_x, const int base_y)
{
    hdc_rectangle(base_x - 75, base_y + 160, 25, 30, 0, 1, 3, CLOTHES_COLOR);
    hdc_rectangle(base_x + 50, base_y + 160, 25, 30, 0, 1, 3, CLOTHES_COLOR);
    hdc_ellipse(base_x + 70, base_y + 196, 20, 6, 0, 1, 3, SIDE_COLOR);
    hdc_ellipse(base_x - 70, base_y + 196, 20, 6, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x - 70, base_y + 190, 20, 12, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x + 50, base_y + 190, 20, 12, 0, 1, 3, SIDE_COLOR);
}

//美队盾牌
static void draw_shield(const int base_x, const int base_y)
{
    int x = base_x - 180, y = base_y + 10, r = 25;
    hdc_circle(x, y, 100, 1, 3, SHIELD_COLOR1);
    hdc_circle(x, y, 75, 1, 3, SHIELD_COLOR2);
    hdc_circle(x, y, 50, 1, 3, SHIELD_COLOR1);
    hdc_circle(x, y, 30, 1, 3, SHIELD_COLOR3);
    draw_star(x, y - r, r);
}

//牌子内容
static void draw_board(const int base_x, const int base_y)
{
    int ey;
    ey = base_y - HEAD_B;
    hdc_rectangle(base_x - HEAD_A - 5, ey + 190, 2 * HEAD_A + 15, 150, 0, 1, 3, BOARD_COLOR);
    /* F */
    hdc_line(base_x - HEAD_A + 30, ey + 220, base_x - HEAD_A + 55, ey + 223, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 30, ey + 220, base_x - HEAD_A + 27, ey + 265, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 30, ey + 245, base_x - HEAD_A + 55, ey + 248, 12, SIDE_COLOR);
    /* U */
    hdc_line(base_x - HEAD_A + 73, ey + 220, base_x - HEAD_A + 70, ey + 265, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 92, ey + 220, base_x - HEAD_A + 95, ey + 265, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 70, ey + 265, base_x - HEAD_A + 95, ey + 265, 12, SIDE_COLOR);
    /* NN */
    for (int i = 0; i < 2; i++)
    {
        hdc_line(base_x - HEAD_A + 110 + i * 45, ey + 220, base_x - HEAD_A + 112 + i * 45, ey + 265, 12, SIDE_COLOR);
        hdc_line(base_x - HEAD_A + 140 + i * 45, ey + 220, base_x - HEAD_A + 138 + i * 45, ey + 265, 12, SIDE_COLOR);
        hdc_line(base_x - HEAD_A + 110 + i * 45, ey + 220, base_x - HEAD_A + 140 + i * 45, ey + 220, 12, SIDE_COLOR);
    }
    /* Y */
    hdc_line(base_x - HEAD_A + 200, ey + 220, base_x - HEAD_A + 213, ey + 240, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 230, ey + 220, base_x - HEAD_A + 213, ey + 240, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 213, ey + 240, base_x - HEAD_A + 213, ey + 265, 12, SIDE_COLOR);
    /* ? */
    hdc_arc(base_x - HEAD_A + 240, ey + 270, 15, -50, 150, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 244, ey + 280, base_x - HEAD_A + 244, ey + 300, 12, SIDE_COLOR);
    hdc_circle(base_x - HEAD_A + 244, ey + 315, 6, 1, 3, SIDE_COLOR);
    /* <-- */
    hdc_line(base_x - HEAD_A + 35, ey + 305, base_x - HEAD_A + 135, ey + 305, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 35, ey + 305, base_x - HEAD_A + 47, ey + 290, 12, SIDE_COLOR);
    hdc_line(base_x - HEAD_A + 35, ey + 305, base_x - HEAD_A + 53, ey + 320, 12, SIDE_COLOR);
}

//另一个举牌子的小黄人
static void draw_another(const int base_x, const int base_y)
{
    int ey;
    ey = base_y - HEAD_B;
    /* body */
    hdc_ellipse(base_x, ey + HEAD_B / 5 + 300, HEAD_A - 20, 55, 0, 1, 3, JEANS_COLOR);
    hdc_rectangle(base_x - HEAD_A + 20, ey + HEAD_B / 5, 2 * (HEAD_A - 20), 300, 0, 1, 3, FACE_COLOR);
    /* hair */
    hdc_arc(base_x + 50, ey - 4 * HEAD_B / 5, 50, 270, 330, 2, SIDE_COLOR);
    hdc_arc(base_x - 50, ey - 4 * HEAD_B / 5, 50, 30, 90, 2, SIDE_COLOR);
    hdc_arc(base_x + 70, ey - 4 * HEAD_B / 5, 70, 270, 310, 2, SIDE_COLOR);
    hdc_arc(base_x - 100, ey - 4 * HEAD_B / 5, 100, 65, 90, 2, SIDE_COLOR);
    hdc_arc(base_x + 80, ey - 4 * HEAD_B / 5, 80, 270, 305, 2, SIDE_COLOR);
    hdc_arc(base_x + 150, ey - 4 * HEAD_B / 5, 150, 270, 290, 2, SIDE_COLOR);
    /* face */
    hdc_ellipse(base_x, ey + HEAD_B / 5, HEAD_A - 20, HEAD_B + 5, 0, 1, 3, FACE_COLOR);
    /* glass */
    hdc_circle(base_x, ey - HEAD_B / 10 + 20, 40, 0, 10, GLASS_COLOR);
    hdc_rectangle(base_x - HEAD_A + 20, ey - HEAD_B / 10 + 15, HEAD_A - 66, 12, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x + 46, ey - HEAD_B / 10 + 15, HEAD_A - 66, 12, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x - 52, ey - HEAD_B / 10 + 9, 10, 24, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x + 43, ey - HEAD_B / 10 + 9, 10, 24, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x - 52, ey - HEAD_B / 10 + 12, 3, 18, 0, 1, 3, GLASS_COLOR);
    hdc_rectangle(base_x + 49, ey - HEAD_B / 10 + 12, 3, 18, 0, 1, 3, GLASS_COLOR);
    /* eyes */
    hdc_sector(base_x, ey - HEAD_B / 10 + 20, 34, 90, 270, 1, 3, EYE_COLOR);
    hdc_sector(base_x, ey - HEAD_B / 10 + 20, 25, 90, 270, 1, 3, BALL_COLOR);
    hdc_sector(base_x, ey - HEAD_B / 10 + 20, 15, 90, 270, 1, 3, SIDE_COLOR);
    hdc_ellipse(base_x + 11, ey - HEAD_B / 10 + 23, 3, 4, 0, 1, 3, EYE_COLOR);
    /* mouse */
    hdc_arc(base_x, ey + 190, 60, -15, 20, 4, SIDE_COLOR);
    /* shose */
    hdc_rectangle(base_x - 50, base_y + 260, 25, 30, 0, 1, 3, JEANS_COLOR);
    hdc_rectangle(base_x + 25, base_y + 260, 25, 30, 0, 1, 3, JEANS_COLOR);
    hdc_ellipse(base_x + 45, base_y + 296, 20, 6, 0, 1, 3, SIDE_COLOR);
    hdc_ellipse(base_x - 45, base_y + 296, 20, 6, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x - 45, base_y + 290, 20, 12, 0, 1, 3, SIDE_COLOR);
    hdc_rectangle(base_x + 25, base_y + 290, 20, 12, 0, 1, 3, SIDE_COLOR);
    /* board */
    draw_board(base_x, base_y);
    /* hand */
    hdc_circle(base_x - HEAD_A + 267, ey + 320, 6, 1, 3, SIDE_COLOR);
    hdc_circle(base_x - HEAD_A - 7, ey + 320, 6, 1, 3, SIDE_COLOR);
    hdc_ellipse(base_x - HEAD_A + 267, ey + 313, 10, 6, 0, 1, 3, SIDE_COLOR);
    hdc_ellipse(base_x - HEAD_A - 7, ey + 313, 10, 6, 0, 1, 3, SIDE_COLOR);
    hdc_ellipse(base_x - HEAD_A + 266, ey + 305, 12, 6, 0, 1, 3, SIDE_COLOR);
    hdc_ellipse(base_x - HEAD_A - 6, ey + 305, 12, 6, 0, 1, 3, SIDE_COLOR);
}

//挑眉
static void wink(const int base_x, const int base_y)
{
    int ey, i;
    ey = base_y - HEAD_B;
    for (i = 0; i < 3; i++)
    {
        //眼睛移动
        hdc_sector(base_x, ey - HEAD_B / 10 + 20, 34, 90, 270, 1, 3, EYE_COLOR);
        hdc_sector(base_x - 6, ey - HEAD_B / 10 + 20, 25, 90, 270, 1, 3, BALL_COLOR);
        hdc_sector(base_x - 6, ey - HEAD_B / 10 + 20, 15, 90, 270, 1, 3, SIDE_COLOR);
        hdc_ellipse(base_x - 17, ey - HEAD_B / 10 + 23, 3, 4, 0, 1, 3, EYE_COLOR);
        Sleep(400);
        hdc_sector(base_x, ey - HEAD_B / 10 + 20, 34, 90, 270, 1, 3, EYE_COLOR);
        hdc_sector(base_x, ey - HEAD_B / 10 + 20, 25, 90, 270, 1, 3, BALL_COLOR);
        hdc_sector(base_x, ey - HEAD_B / 10 + 20, 15, 90, 270, 1, 3, SIDE_COLOR);
        hdc_ellipse(base_x + 11, ey - HEAD_B / 10 + 23, 3, 4, 0, 1, 3, EYE_COLOR);
        Sleep(300);
    }
}

void hdc_draw_cartoon_2153592(const int base_x, const int base_y)
{
    hdc_cls();
    cct_setconsoleborder(200, 150);
    background(base_x, base_y);
    //Minion with the clothes of Captain America
    draw_hair(base_x, base_y);
    draw_head_face_cheek_body(base_x, base_y);
    draw_mouth(base_x, base_y);
    draw_eyes(base_x, base_y);
    draw_flag(base_x, base_y);
    draw_clothes(base_x, base_y);
    draw_arm(base_x, base_y);
    draw_legs(base_x, base_y);
    draw_shield(base_x, base_y);
    //Minion
    draw_another(base_x + 375, base_y - 100);
    Sleep(200);
    wink(base_x + 375, base_y - 100);
}