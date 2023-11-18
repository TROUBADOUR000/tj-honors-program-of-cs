/* 信01 2153592 胡逸凡 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     本文件功能：
    1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
    1、不允许定义外部全局变量（const及#define不在限制范围内）
    2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
    3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
    4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */
char src, dst, tmp;
int a[10], b[10], c[10], p = 0, q = 0, r = 0, j, i = 0, n;
static int v = 0;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

void input(int z)
{
    cout << "请输入汉诺塔的层数(1-10)" << endl;
    cin >> n;
    while (cin.good() == 0 || n < 1 || n>16)
    {
        cin.clear();
        cin.ignore(100000, '\n');
        cout << "请输入汉诺塔的层数(1-10)" << endl;
        cin >> n;
    }
    cin.clear();
    cin.ignore(100000, '\n');
    cout << "请输入起始柱(A-C)" << endl;
    cin >> src;
    while (src != 'A' && src != 'B' && src != 'C'
        && src != 'a' && src != 'b' && src != 'c' || cin.good() == 0)
    {
        cin.clear();
        cin.ignore(100000, '\n');
        cout << "请输入起始柱(A-C)" << endl;
        cin >> src;
    }
    if (int(src) == int('a') || int(src) == int('b') || int(src) == int('c'))
        src = char(int(src) - 32);
    cin.clear();
    cin.ignore(100000, '\n');
    cout << "请输入目标柱(A-C)" << endl;
    cin >> dst;
    while (dst != 'A' && dst != 'B' && dst != 'C' && dst != 'a'
        && dst != 'b' && dst != 'c' || dst == src || dst == src + 32 || cin.good() == 0)
    {
        if (int(dst) == int('a') || int(dst) == int('b') || int(dst) == int('c'))
            dst = char(int(dst) - 32);
        if (dst == src)
            cout << "目标柱(" << src << ")不能与起始柱(" << src << ")相同" << endl;
        cin.clear();
        cin.ignore(100000, '\n');
        cout << "请输入目标柱(A-C)" << endl;
        cin >> dst;
    }
    cin.clear();
    cin.ignore(100000, '\n');
    if (int(dst) == int('a') || int(dst) == int('b') || int(dst) == int('c'))
        dst = char(int(dst) - 32);
    tmp = char(int('A') + int('B') + int('C') - int(dst) - int(src));
    if (z == 4 || z == 8)
        while (1)
        {
            cout << "请输入移动速度(0-5: 0-按回车单步演示 1-延时最长 5-延时最短) ";
            cin >> v;
            if (v < 0 || v>5 || cin.good() == 0)
            {
                cin.clear();
                cin.ignore(100000, '\n');
            }
            else
                break;
        }
}
void judge()
{
    if (v != 0)
        Sleep(40 * (6 - v));
    else
    {
        while (1)
            if (_getch() == 13)
                break;
    }
}
void init(int n, char src)
{
    if (src == 'A')
        for (j = n; j > 0; j--)
            a[p++] = j;
    else if (src == 'B')
        for (j = n; j > 0; j--)
            b[q++] = j;
    else
        for (j = n; j > 0; j--)
            c[r++] = j;
}
void clear()
{
    for (j = 0; j < 10; j++)
    {
        a[j] = 0;
        b[j] = 0;
        c[j] = 0;
    }
    p = 0;
    q = 0;
    r = 0;
    i = 0;
}
void shuchu()
{
    cout << "A:";
    for (j = 0; j < p; j++)
        cout << setw(2) << a[j];
    cout << setw(23 - 2 * p) << "B:";
    for (j = 0; j < q; j++)
        cout << setw(2) << b[j];
    cout << setw(23 - 2 * q) << "C:";
    for (j = 0; j < r; j++)
        cout << setw(2) << c[j];
    cout << setw(20 - 2 * r) << ' ' << endl;
}
void change(char src, char dst)
{
    if (src == 'A' && dst == 'B')
    {
        b[q++] = a[--p];
        a[p] = 0;
    }
    else if (src == 'A' && dst == 'C')
    {
        c[r++] = a[--p];
        a[p] = 0;
    }
    else if (src == 'B' && dst == 'A')
    {
        a[p++] = b[--q];
        b[q] = 0;
    }
    else if (src == 'B' && dst == 'C')
    {
        c[r++] = b[--q];
        b[q] = 0;
    }
    else if (src == 'C' && dst == 'A')
    {
        a[p++] = c[--r];
        c[r] = 0;
    }
    else if (src == 'C' && dst == 'B')
    {
        b[q++] = c[--r];
        c[r] = 0;
    }
}
void rechange(char src, char dst)
{
    if (src == 'A' && dst == 'B')
    {
        a[p++] = b[--q];
        b[q] = 0;
    }
    else if (src == 'A' && dst == 'C')
    {
        a[p++] = c[--r];
        c[r] = 0;
    }
    else if (src == 'B' && dst == 'A')
    {
        b[q++] = a[--p];
        a[p] = 0;
    }
    else if (src == 'B' && dst == 'C')
    {
        b[q++] = c[--r];
        c[r] = 0;
    }
    else if (src == 'C' && dst == 'A')
    {
        c[r++] = a[--p];
        a[p] = 0;
    }
    else if (src == 'C' && dst == 'B')
    {
        c[r++] = b[--q];
        b[q] = 0;
    }
}
void shuchu1(int f)
{
    int j;
    if (i != 0)
        cct_gotoxy(22, f);
    cout << "A:";
    for (j = 0; j < p; j++)
        cout << setw(2) << a[j];
    cout << setw(23 - 2 * p) << "B:";
    for (j = 0; j < q; j++)
        cout << setw(2) << b[j];
    cout << setw(23 - 2 * q) << "C:";
    for (j = 0; j < r; j++)
        cout << setw(2) << c[j];
    cout << setw(20 - 2 * r) << ' ';
}
void shuchu2(int n, char src, char dst)
{
    cct_gotoxy(0, 0);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为 " << v;
}
void shuchu3(int g)
{
    int j;
    cct_gotoxy(9, g);
    cout << setw(25) << setfill('=') << "=" << endl;
    cout << setw(12) << setfill(' ') << "A" << setw(10) << "B" << setw(10) << "C";
    for (j = 0; j < p; j++)
    {
        cct_gotoxy(10, g - p + j);
        cout << setw(2) << a[p - j - 1];
    }
    for (j = 0; j < q; j++)
    {
        cct_gotoxy(20, g - q + j);
        cout << setw(2) << b[q - j - 1];
    }
    for (j = 0; j < r; j++)
    {
        cct_gotoxy(30, g - r + j);
        cout << setw(2) << c[r - j - 1];
    }
}
void shuchu4(char src, char dst, int f)
{
    cct_gotoxy(0, f);
    cout << "第" << setw(4) << i << "步(" << setw(2) << 1 << "#: " << src << "-->" << dst << ")  ";
}
void move(int n, char src, char dst)
{
    int h, l, k;
    if (src == 'A')
    {
        h = p;
        k = a[p - 1];
    }
    else if (src == 'B')
    {
        h = q;
        k = b[q - 1];
    }
    else
    {
        h = r;
        k = c[r - 1];
    }
    if (dst == 'A')
        l = p;
    else if (dst == 'B')
        l = q;
    else
        l = r;
    int w, u, t[3] = { 12,44,76 };
    for (u = 16 - h; u >= 2; u--)
    {
        cct_showch(t[int(src) - 65] - k, u, ' ', 14 - k, 1, 2 * k + 1);
        Sleep(10 * (6 - v));
        if (u >= 2)
        {
            cct_showch(t[int(src) - 65] - k, u, ' ', 0, 7, 2 * k + 1);
            if (u >= 4)
                cct_showch(t[int(src) - 65], u, ' ', 14, 1, 1);
        }
    }
    if (dst > src)
    {
        for (w = t[int(src) - 65] - k; w <= t[int(dst) - 65] - k; w++)
        {
            cct_showch(w, 2, ' ', 14 - k, 1, 2 * k + 1);
            Sleep(10 * (6 - v));
            cct_showch(w, 2, ' ', 0, 7, 2 * k + 1);
        }
    }
    else
    {
        for (w = t[int(src) - 65] - k; w >= t[int(dst) - 65] - k; w--)
        {
            cct_showch(w, 2, ' ', 14 - k, 1, 2 * k + 1);
            Sleep(10 * (6 - v));
            cct_showch(w, 2, ' ', 0, 7, 2 * k + 1);
        }
    }
    for (u = 2; u <= 15 - l; u++)
    {
        cct_showch(t[int(dst) - 65] - k, u, ' ', 14 - k, 1, 2 * k + 1);

        Sleep(10 * (6 - v));
        if (u >= 2 && u <= 14 - l)
        {
            cct_showch(t[int(dst) - 65] - k, u, ' ', 0, 7, 2 * k + 1);
            if (u >= 4)
                cct_showch(t[int(dst) - 65], u, ' ', 14, 1, 1);
        }
    }
    cct_setcolor();
}
void output(int n, char src, char dst, int f, int z)
{
    i++;
    int g;
    g = f - 5;
    shuchu4(src, dst, f);
    if (v == 0)
        shuchu3(g);
    if (src == 'A')
        cct_gotoxy(10, g - p);
    else if (src == 'B')
        cct_gotoxy(20, g - q);
    else
        cct_gotoxy(30, g - r);
    cout << "    ";
    change(src, dst);
    shuchu1(f);
    if (dst == 'A')
    {
        cct_gotoxy(10, g - p);
        cout << setw(2) << a[p - 1];
    }
    else if (dst == 'B')
    {
        cct_gotoxy(20, g - q);
        cout << setw(2) << b[q - 1];
    }
    else
    {
        cct_gotoxy(30, g - r);
        cout << setw(2) << c[r - 1];
    }
    shuchu3(g);
    rechange(src, dst);
    if (z == 8)
        move(n, src, dst);
    change(src, dst);
    judge();
}
void disk(int z)
{
    int w, u, t[3] = { 12,44,76 }, d;
    for (w = 1; w <= 65; w += 32)
        cct_showch(w, 16, ' ', 14, 1, 23);
    for (u = 15; u >= 4; u--)
        for (w = 12; w <= 76; w += 32)
        {
            cct_showch(w, u, ' ', 14, 1, 1);
            Sleep(20);
        }
    if (z != 5)
        for (d = n; d >= 1; d--)
        {
            cct_showch(t[int(src) - 65] - d, 15 + d - n, ' ', 14 - d, 1, 2 * d + 1);
            Sleep(15);
        }
}

void hh(int n, char src, char tmp, char dst, int f, int z)
{
    if (z == 1 || z == 2 || z == 3)
    {
        i++;
        if (z == 1)
            cout << n << "# " << src << "---->" << dst << endl;
        else if (z == 2 || z == 3)
        {
            cout << "第" << setw(4) << setfill(' ') << i << " 步(" << setw(2) << n << "#: " << src << "-->" << dst << ")  ";
            if (z == 3)
            {
                change(src, dst);
                shuchu();
            }
            else
                cout << endl;
        }
    }
    else
        output(n, src, dst, f, z);
}
void hanoi(int n, char src, char tmp, char dst, int f, int z)
{
    if (n == 1)
        hh(n, src, tmp, dst, f, z);
    else
    {
        hanoi(n - 1, src, dst, tmp, f, z);
        hh(n, src, tmp, dst, f, z);
        hanoi(n - 1, tmp, src, dst, f, z);
    }
}

void fun1(int z)
{
    input(z);
    hanoi(n, src, tmp, dst, 0, z);
    clear();
}
void fun2(int z)
{
    input(z);
    hanoi(n, src, tmp, dst, 0, z);
    clear();
}
void fun3(int z)
{
    input(z);
    init(n, src);
    hanoi(n, src, tmp, dst, 0, z);
    clear();
}
void fun4(int z)
{
    int f;
    f = 17;
    input(z);
    cct_setcursor(CURSOR_INVISIBLE);
    init(n, src);
    cct_cls();
    shuchu2(n, src, dst);
    cct_gotoxy(0, f);
    cout << "初始:" << setw(16) << ' ';
    shuchu1(f);
    shuchu2(n, src, dst);
    shuchu3(f - 5);
    judge();
    hanoi(n, src, tmp, dst, f, z);
    cct_gotoxy(0, 37);
    clear();
}
void fun5(int z)
{
    cct_setcursor(CURSOR_INVISIBLE);
    cct_cls();
    disk(z);
    cct_setcolor();
    cct_gotoxy(0, 37);
}
void fun6(int z)
{
    input(z);
    cct_setcursor(CURSOR_INVISIBLE);
    cct_cls();
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层";
    disk(z);
    cct_setcolor();
    cct_gotoxy(0, 37);
}
void fun7(int z)
{
    input(z);
    cct_setcursor(CURSOR_INVISIBLE);
    init(n, src);
    int t[3] = { 12,44,76 };
    cct_cls();
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层";
    disk(z);
    Sleep(200);
    if (n % 2 == 1)
        move(n, src, dst);
    else
        move(n, src, tmp);
    cct_gotoxy(0, 37);
}
void fun8(int z)
{
    int f;
    f = 33;
    input(z);
    cct_setcursor(CURSOR_INVISIBLE);
    init(n, src);
    cct_cls();
    shuchu2(n, src, dst);
    cct_gotoxy(0, f);
    cout << "初始:" << setw(16) << ' ';
    shuchu1(f);
    shuchu3(f - 5);
    disk(z);
    cct_setcolor();
    judge();
    hanoi(n, src, tmp, dst, f, z);
    cct_gotoxy(0, 37);
    clear();
}
void fun9(int z)
{
    int f, t[3] = { 12,44,76 }, a4, j = 0;
    char a1, a2, a3, ch = 0;
    int X = 0, Y = 0;
    int maction;
    int keycode1, keycode2;
    f = 33;
    input(z);
    a4 = n;
    a3 = dst;
    cct_setcursor(CURSOR_INVISIBLE);
    init(n, src);
    cct_cls();
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层";
    cct_gotoxy(0, f);
    cout << "初始:" << setw(16) <<setfill(' ')<< ' ';
    shuchu1(f);
    shuchu3(f - 5);
    disk(z);
    cct_setcolor();
    cct_gotoxy(0, 35);
    cct_setcursor(CURSOR_VISIBLE_NORMAL);
    cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
    while (1)
    {
        while (1)
        {
            j = 0;
            while (j < 20)
            {
                cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
                if (j == 0)
                    a1 = char(keycode1);
                else if (j == 1)
                    a2 = char(keycode1);
                if (!keycode2)
                {
                    if (keycode1 == 13)
                        break;
                    else
                    {
                        if (keycode1 > ' ' && keycode1 < 127)
                            cout << char(keycode1);
                        else
                            j--;

                    }
                }
                j++;
            }
            if ((a1 == 'q' || a1 == 'Q') && j == 1)
                break;
            else if ((a1 == 'A' || a1 == 'a' || a1 == 'B' || a1 == 'b' || a1 == 'C' || a1 == 'c') && (a2 == 'A'
                || a2 == 'a' || a2 == 'B' || a2 == 'b' || a2 == 'C' || a2 == 'c') && a1 != a2 && a1 != a2 + 32 && a1 != a2 - 32 && j == 2)
            {
                if (a1 >= 97)
                    a1 -= 32;
                if (a2 >= 97)
                    a2 -= 32;
                break;
            }
            else
            {
                cct_gotoxy(60, 35);
                cout << setw(20) << ' ';
                cct_gotoxy(60, 35);
            }
        }
        if (a1 == 'A' && p == 0 || a1 == 'B' && q == 0 || a1 == 'C' && r == 0)
        {
            cct_gotoxy(0, 36);
            cout << "源柱为空!";
            Sleep(800);
            cct_gotoxy(0, 36);
            cout << setw(9) << ' ';
            cct_gotoxy(60, 35);
            cout << setw(20) << ' ';
            cct_gotoxy(60, 35);
            continue;
        }
        if (a1 == 'A' && a2 == 'B' && a[p - 1] > b[q - 1] && b[q - 1] != 0 || a1 == 'A' && a2 == 'C' && a[p - 1] > c[r - 1] && c[r - 1] != 0 ||
            a1 == 'B' && a2 == 'A' && b[q - 1] > a[p - 1] && a[p - 1] != 0 || a1 == 'B' && a2 == 'C' && b[q - 1] > c[r - 1] && c[r - 1] != 0 ||
            a1 == 'C' && a2 == 'A' && c[r - 1] > a[p - 1] && a[p - 1] != 0 || a1 == 'C' && a2 == 'B' && c[r - 1] > b[q - 1] && b[q - 1] != 0)
        {
            cct_gotoxy(0, 36);
            cout << "大盘压小盘，非法移动!";
            Sleep(800);
            cct_gotoxy(0, 36);
            cout << setw(21) << ' ';
            cct_gotoxy(60, 35);
            cout << setw(20) << ' ';
            cct_gotoxy(60, 35);
            continue;
        }
        if (a1 == 'q' || a1 == 'Q')
        {
            cct_gotoxy(0, 36);
            cout << "游戏终止!!!!!";
            break;
        }
        v = 6;
        cct_setcursor(CURSOR_INVISIBLE);
        output(n, a1, a2, f, 8);
        if (a3 == 'A' && p == a4 || a3 == 'B' && q == a4 || a3 == 'C' && r == a4)
        {
            cct_gotoxy(0, 36);
            cout << "游戏结束!!!!!";
            break;
        }
        cct_gotoxy(60, 35);
        cout << setw(20) << ' ';
        cct_gotoxy(60, 35);
    }
    cct_gotoxy(0, 38);
    clear();
}