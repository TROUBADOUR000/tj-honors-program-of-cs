/* 2153592 ��01 ���ݷ� */
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
using namespace std;

#define _max 1000
#define _num 14
#define length 20

void find()
{
    _finddata_t file;
    int k;
    long HANDLE;
    k = HANDLE = long(_findfirst("sudoku*.txt", &file));
    while (k != -1)
    {
        cout << file.name << ' ';
        k = _findnext(HANDLE, &file);
    }
    _findclose(HANDLE);
}

void find_list(char name[_num][length])
{
    _finddata_t file;
    int k, i = 0;
    long HANDLE;
    k = HANDLE = long(_findfirst("sudoku*.txt", &file));
    while (k != -1 && i < _num)
    {
        strcpy_s(*(name + i), file.name);
        k = _findnext(HANDLE, &file);
        i++;
    }
    _findclose(HANDLE);
}

int by_keyboard()
{
    int ret, maction, keycode1, keycode2, loop = 1, X = 0, Y = 0, t = 0;
    cct_setcursor(CURSOR_INVISIBLE);
    while (loop)
    {
        ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
        if (ret == CCT_KEYBOARD_EVENT)
        {
            switch (keycode1)
            {
            case 13:	//�س���
            {
                t = 0;
                loop = 0;
                break;
            }
            case 224:
                switch (keycode2)
                {
                case KB_ARROW_UP:
                {
                    t = 1;
                    loop = 0;
                    break;
                }
                case KB_ARROW_DOWN:
                {
                    t = 2;
                    loop = 0;
                    break;
                }
                default:
                    break;
                }
                break;
            default:	//����������ʾ
                break;
            }//end of swicth(keycode1)
        }//end of else if(ret == CCT_KEYBOARD_EVENT��
    } //end of while(1)
    cct_setcursor(CURSOR_VISIBLE_NORMAL);	//�򿪹��
    return t;
}

void write(int t, char name[_num][length])
{
    int i;
    for (i = 0; i < 8; i++)
    {
        cct_gotoxy(65, i + 4);
        cout << "�U" << setw(length) << setiosflags(ios::left) << setfill(' ') << name[i + t];
        cct_gotoxy(87, i + 4);
        cout << "�U";
    }
}

void read_in(char filename[length], char name[_num][length])
{
    int i = 0, loop = 1, x, y;
    cct_gotoxy(67, 4);
    cct_setcolor(15, 0);
    cout << setw(length) << setiosflags(ios::left) << setfill(' ') << name[0];
    cct_setcolor(0, 15);
    while (loop)
        switch (by_keyboard())
        {
        case 0:
        {
            strcpy(filename, *(name + i));
            loop = 0;
            break;
        }
        case 1:
            if (i > 0 && i < _num)
            {
                cct_getxy(x, y);
                if (y == 4)
                {
                    i--;
                    write(i, name);
                    cct_gotoxy(67, 4);
                    cct_setcolor(15, 0);
                    cout << setw(length) << setiosflags(ios::left) << setfill(' ') << name[i];
                    cct_setcolor(0, 15);
                }
                else
                {
                    cct_gotoxy(67, y);
                    cct_setcolor(0, 15);
                    cout << setw(length) << setiosflags(ios::left) << setfill(' ') << name[i];
                    i--;
                    cct_gotoxy(67, y - 1);
                    cct_setcolor(15, 0);
                    cout << setw(length) << setiosflags(ios::left) << setfill(' ') << name[i];
                    cct_setcolor(0, 15);
                }
            }
            break;
        case 2:
            if (i >= 0 && i < _num - 1)
            {
                cct_getxy(x, y);
                if (y == 11)
                {
                    i++;
                    write(i - 7, name);
                    cct_gotoxy(67, 11);
                    cct_setcolor(15, 0);
                    cout << setw(length) << setiosflags(ios::left) << setfill(' ') << name[i];
                    cct_setcolor(0, 15);
                }
                else
                {
                    cct_gotoxy(67, y);
                    cct_setcolor(0, 15);
                    cout << setw(length) << setiosflags(ios::left) << setfill(' ') << name[i];
                    i++;
                    cct_gotoxy(67, y + 1);
                    cct_setcolor(15, 0);
                    cout << setw(length) << setiosflags(ios::left) << setfill(' ') << name[i];
                    cct_setcolor(0, 15);
                }
            }
            break;
        }
}

void file(int a[9][9], int choice)
{
    int i, j, t;
    char filename[length] = { '\0' }, name[_num][length];
    const char f[length] = "sudoku.txt";
    cct_cls();
    if(choice==1)
        while (1)
        {
            t = 1;
            cout << "��ǰĿ¼�·���sudoku*.txt��ƥ���ļ��У�" << endl;
            find();
            cout << "������������Ŀ�ļ���(�س���ʾĬ��sudoku.txt)��";
            cin.get(filename, length, '\n');
            cin.clear();
            cin.ignore(1024, '\n');
            if (filename[0] == 0)
                strcpy_s(filename, f);
            ifstream infile(filename, ios::in);
            if (!infile.is_open())
            {
                cout << "��������Ŀ�ļ�[" << filename << "]ʧ�ܣ�����������" << endl;
                cout << endl;
            }
            else
            {
                cout << "��������Ŀ�ļ�[" << filename << "]�ɹ�" << endl;
                for (i = 0; i < 9; i++)
                    for (j = 0; j < 9; j++)
                    {
                        infile >> a[i][j];
                        if (a[i][j] > 9 || a[i][j] < 0)
                        {
                            t = 0;
                            break;
                        }
                    }
                if (t == 1)
                    break;
                else
                    cout << "������Ŀ�ļ�[" << filename << "]�зǷ����֣�����������\n" << endl;
            }
        }
    if (choice == 2)
        while (1)
        {
            t = 1;
            find_list(name);
            cct_gotoxy(65, 2);
            cout << "���������ļ�" << endl;
            cct_gotoxy(65, 3);
            cout << "�X�T�T�T�T�T�T�T�T�T�T�[" << endl;
            write(0, name);
            cct_gotoxy(65, 12);
            cout << "�^�T�T�T�T�T�T�T�T�T�T�a" << endl;                
            read_in(filename, name);
            ifstream infile(filename, ios::in);
            if (!infile.is_open())
            {
                cout << "��������Ŀ�ļ�[" << filename << "]ʧ�ܣ�����������" << endl;
                cout << endl;
            }
            else
            {
                for (i = 0; i < 9; i++)
                    for (j = 0; j < 9; j++)
                    {
                        infile >> a[i][j];
                        if (a[i][j] > 9 || a[i][j] < 0)
                        {
                            t = 0;
                            break;
                        }
                    }
                if (t == 1)
                    break;
                else
                {
                    cct_gotoxy(0, 25);
                    cout << "������Ŀ�ļ�[" << filename << "]�зǷ����֣����س�������" << endl;
                    while (getchar() != '\n');
                    cct_cls();
                }
            }
        }
}

void character_pattern(int choice)
{
    struct info
    {
        int fx, num;
        char fy;
    };
    info* p;
    p = new(nothrow) info[_max];
    if (p != NULL)
    {
        int a[9][9], i, j, t, s = 0, * steps = &s, x, n, * nx = &x, * nu = &n, sum;
        char flag[9][9], y, * ny = &y, fla[9][9];
        file(a, choice);
        if (choice == 1)
            cout << "���������Ϊ��" << endl;
        for (i = 0; i < 9; i++)
            for (j = 0; j < 9; j++)
            {
                if (a[i][j] == 0)
                    flag[i][j] = '0';
                else
                    flag[i][j] = '*';
            }
        sum = is_sudoku(a, fla);
        if (sum != 0)
        {
            if (choice == 1)
            {
                cout << "�����������" << sum << "����ͻ�������ǣ�" << endl;
                print(a, flag, fla);
                cout << "ԭʼ�����ļ��д����޸���ȷ�����" << endl;
            }
            else
            {
                cct_gotoxy(4, 1);
                draw(a, flag, fla);
                cct_gotoxy(0, 30);
                cout << "�����ļ���" << sum << "����ͻ�����޸���ȷ�����" << endl;
            }
        }
        else
        {
            if (choice == 1)
                print(a, flag, fla);
            else
            {
                cct_gotoxy(4, 1);
                draw(a, flag, fla);
            }
            while (1)
            {
                if (is_solution(a, fla) == 1)
                {
                    if (choice == 2)
                        cout << endl;
                    cout << "����������Ϸ���!!!!!" << endl;
                    break;
                }
                if (s > 0)
                {
                    *nx = p[(s - 1) % _max].fx;
                    *ny = p[(s - 1) % _max].fy;
                    *nu = p[(s - 1) % _max].num;
                }
                if (choice == 2)
                    cout << endl;
                t = input(a, flag, steps, nx, ny, nu, fla, choice);
                if (choice == 1)
                {
                    if (t == 0 || t == 8 || t > 9)
                        print(a, flag, fla);
                }
                else if (choice == 2)
                {
                    if (t == 0 || t == 8 || t > 8)
                        draw(a, flag, fla);
                    if (t > 8 || t == 8)
                    {
                        if (t > 8 && t - 9 == 0 && a[*nx - 1][*ny - 'a'] == 0);
                        else
                            single(a[*nx - 1][*ny - 'a'], '0', 6 * (*ny - 'a') + 6, 3 * *nx - 1, '#', 2);
                    }
                }
                if (t != 0 && p != NULL && t < 9)
                {
                    p[s % _max].fx = *nx;
                    p[s % _max].fy = *ny;
                    p[s % _max].num = *nu;
                    s++;
                }
            }
        }
    }
    delete[]p;
}

int count(int a[9][9], char flag[9][9], int x, int y, int able[9], int choice)
{
    int n = 0, unable[9], i, j;
    if (a[x][y] > 0)
        return 10;
    else
    {
        for (i = 0; i < 9; i++)
            unable[i] = 0;
        for (i = 0; i < 9; i++)
        {
            if (a[x][i] > 0)
                unable[a[x][i] - 1]--;
            if (a[i][y] > 0)
                unable[a[i][y] - 1]--;
        }
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                if (a[x / 3 * 3 + i][y / 3 * 3 + j] > 0)
                    unable[a[x / 3 * 3 + i][y / 3 * 3 + j] - 1]--;
        for (i = 0; i < 9; i++)
            if (unable[i] == 0)
            {
                n++;
                if (choice == 2)
                    able[i] = 1;
            }
        return n;
    }
}

int find_min(int a[9][9], char flag[9][9], int* nx, char* ny)
{
    int i, j, n, min = 9, c[9];
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
        {
            n = count(a, flag, i, j, c, 1);
            if (min > n)
            {
                min = n;
                *nx = i + 1;
                *ny = char(j + 'a');
            }
        }
    return min;
}

void solutions(int a[9][9], char flag[9][9], char fla[9][9], int* steps, int& t)
{
    int i, able[9] = { 0 }, x, * nx = &x;
    char y, * ny = &y;
    if (is_solution(a, fla) == 1)
    {
        cct_gotoxy(0, 32);
        cout << "����������Ϸ���!!!!!" << endl;
        t = 1;
        return;
    }
    else
        if (find_min(a, flag, nx, ny) == 0)
            return;
        else
        {
            restore_fla(fla);
            count(a, flag, *nx - 1, *ny - 'a', able, 2);
            for (i = 0; i < 9; i++)
                if (able[i] == 1)
                {
                    draw(a, flag, fla);
                    (*steps)++;
                    cct_gotoxy(0, 30);
                    cout << "����������" << *steps;
                    a[*nx - 1][*ny - 'a'] = i + 1;
                    single(a[*nx - 1][*ny - 'a'], '0', 6 * (*ny - 'a') + 6, 3 * *nx - 1, '#', 2);
                    Sleep(50);
                    solutions(a, flag, fla, steps, t);
                    if (is_solution(a, fla) == 1)
                        return;
                }
            if (is_solution(a, fla) == 0)
                a[*nx - 1][*ny - 'a'] = 0;
        }
}

void auto_solution()
{
    struct info
    {
        int fx, num;
        char fy;
    };
    info* p;
    p = new(nothrow) info[_max];
    if (p != NULL)
    {
        int a[9][9], i, j, s = 0, * steps = &s, sum, t = 0;
        char flag[9][9], fla[9][9];
        file(a, 2);
        for (i = 0; i < 9; i++)
            for (j = 0; j < 9; j++)
            {
                if (a[i][j] == 0)
                    flag[i][j] = '0';
                else
                    flag[i][j] = '*';
            }
        sum = is_sudoku(a, fla);
        cct_gotoxy(4, 1);
        draw(a, flag, fla);
        if (sum != 0)
        {
            cct_gotoxy(0, 32);
            cout << "�����ļ���" << sum << "����ͻ�����޸���ȷ�����" << endl;
        }
        else
        {
            solutions(a, flag, fla, steps, t);
            if (t == 0)
            {
                cct_gotoxy(0, 32);
                cout << "����������Ϸ�޽⣡��������" << endl;
            }
        }
    }
    delete[]p;
}

int by_mouse_keyboard(int a[9][9], char f[9][9], int* x1, char* y1, int* n1, char fla[9][9], int* nx, char* ny, int* nu, int choice)
{
    cct_gotoxy(0, 32);
    cout << setw(30) << setfill(' ') << ' ' << endl;
    cout << setw(50) << setfill(' ') << ' ' << endl;
    int ret, maction, keycode1, keycode2, loop = 1, X = 0, Y = 0, u, is_back = 0, flag = 0;
    char v;
    cct_setcursor(CURSOR_INVISIBLE);
    cct_enable_mouse();
    while (loop)
    {
        ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
        if (choice == 1 && ret == CCT_MOUSE_EVENT)
        {
            switch (maction)
            {
            case MOUSE_ONLY_MOVED:
                u = (Y + 1) / 3;
                v = char(X / 6 - 1 + 'a');
                if (u != *x1 || v != *y1)
                    flag = 0;
                if (Y < 29 && Y > 2 && X > 5 && X < 60)
                {
                    draw(a, f, fla);
                    cct_gotoxy(0, 31);
                    cout << setw(30) << setfill(' ') << ' ' << endl;
                    cct_gotoxy(0, 31);
                    cout << "[��ǰλ��] : " << u << "��" << v << "��";
                    temporary(a[u - 1][v - 'a'], 6 * (v - 'a') + 6, 3 * u - 1);
                }
                else if (Y >= 25 && Y <= 28 && X >= 66 && X <= 72)
                {
                    draw(a, f, fla);
                    cct_gotoxy(0, 31);
                    cout << "[��ǰλ��] : backspace";
                }
                else
                {
                    draw(a, f, fla);
                    cct_gotoxy(0, 31);
                    cout << setw(40) << setfill(' ') << ' ';
                    cct_gotoxy(0, 31);
                    cout << "�Ƿ�λ��";
                }
                break;
            case MOUSE_LEFT_BUTTON_CLICK:			//�������            
                if (Y >= 25 && Y <= 28 && X >= 66 && X <= 72)
                {
                    is_back = 1;
                    loop = 0;
                }
                else if(Y < 29 && Y > 2 && X > 5 && X < 60)
                {
                    *x1 = u;
                    *y1 = v;
                    loop = 0;
                }                
                break;
            default:
                break;
            } //end of switch(maction)
        } //end of if (CCT_MOUSE_EVENT)
        else if (choice == 2 && ret == CCT_KEYBOARD_EVENT)
        {
            u = *x1;
            v = *y1;
            flag = 0;
            switch (keycode1)
            {
            case 13:	//�س���
                loop = 0;
                break;
            case 8:     //�˸��
                loop = 0;
                is_back = 1;
                break;
            case 224:
                switch (keycode2)
                {
                case KB_ARROW_UP:
                    if (*x1 > 1)
                        (*x1)--;
                    else
                        *x1 = 9;
                    break;
                case KB_ARROW_DOWN:
                    if (*x1 < 9)
                        (*x1)++;
                    else
                        *x1 = 1;
                    break;
                case KB_ARROW_LEFT:
                    if (*y1 > 'a')
                        (*y1)--;
                    else
                        *y1 = 'i';
                    break;
                case KB_ARROW_RIGHT:
                    if (*y1 < 'i')
                        (*y1)++;
                    else
                        *y1 = 'a';
                    break;
                }
                break;
            default:	//����������ʾ
                break;
            }//end of swicth(keycode1)
            if (u != *x1 || v != *y1)
            {
                cct_gotoxy(0, 32);
                cout << setw(30) << setfill(' ') << ' ' << endl;
                cout << setw(50) << setfill(' ') << ' ' << endl;
                draw(a, f, fla);
            }
            cct_gotoxy(0, 31);
            cout << "[��ǰ����] : " << *x1 << "��" << *y1 << "��";
            temporary(a[*x1 - 1][*y1 - 'a'], 6 * (*y1 - 'a') + 6, 3 * *x1 - 1);
        }//end of else if(ret == CCT_KEYBOARD_EVENT��
    } //end of while(1)
    cct_disable_mouse();	//�������
    cct_setcursor(CURSOR_VISIBLE_NORMAL);	//�򿪹��
    cct_gotoxy(0, 32);
    if (is_back == 0)
    {
        cout << "ѡ��" << *x1 << "��" << *y1 << "�У�������������ֵ��";
        while (1)
        {
            *n1 = _getch();
            if (*n1 - 48 > 0 && *n1 - 48 <= 9)
            {
                cout << *n1 - 48 << endl;
                break;
            }
        }
    }
    else
        cout << setw(30) << setfill(' ') << ' ' << endl;
    return is_back;
}

int _input(int a[9][9], char flag[9][9], int* steps, int* nx, char* ny, int* nu, char fla[9][9], int choice)
{
    int x, n = a[0][0], sum, t, o, * x1 = &x, * n1 = &n, is_back;
    char y, in[100] = { '\0' }, * y1 = &y;
    if (choice == 2)
    {
        x = 1;
        y = 'a';
    }
    cct_gotoxy(0, 30);
    cout << "�밴��ͷ���ƶ�λ�ã����س���ȷ�ϣ���Backspace��ʾ����һ�Σ�";
    is_back = by_mouse_keyboard(a, flag, x1, y1, n1, fla, nx, ny, nu, choice);
    if (is_back == 1)
    {
        if (*steps == 0)
        {
            cout << "����������Ϊ��" << endl;
            Sleep(800);
            return 0;
        }
        else
        {
            cout << "����������ȷ" << endl;
            o = 9 + a[*nx - 1][*ny - 'a'];
            restore_fla(fla);
            a[*nx - 1][*ny - 'a'] = *nu;
            (*steps)--;
            is_sudoku(a, fla);
            return o;
        }
    }
    else
    {
        n -= 48;
        switch (sum = check(x - 1, y, a, n, flag, fla))
        {
        case 9:  
            cout << "��λ������Ŀԭʼֵ�����ܸı䣬����������" << endl;
            Sleep(800);
            return 0;
        case 8:
        {
            cout << "����������ȷ" << endl;
            if (x == *nx && y == *ny)
                restore_fla(fla);
            *nu = a[x - 1][y - 'a'];
            a[x - 1][y - 'a'] = n;
            *nx = x;
            *ny = y;
            return 8;
        }
        default:
        {
            if (sum == 1 || sum == 2 || sum == 4)
                t = 1;
            else if (sum == 7)
                t = 3;
            else
                t = 2;
            cout << "�����������" << t << "����ͻ" << endl;
            *nu = a[x - 1][y - 'a'];
            a[x - 1][y - 'a'] = n;
            *nx = x;
            *ny = y;
            is_sudoku(a, fla);
            draw(a, flag, fla);
            return sum;
            break;
        }
        }
    }
}

void control(int choice)
{
    struct info
    {
        int fx, num;
        char fy;
    };
    info* p;
    p = new(nothrow) info[_max];
    if (p != NULL)
    {
        int a[9][9], i, j, t, s = 0, * steps = &s, x, n, * nx = &x, * nu = &n, sum;
        char flag[9][9], y, * ny = &y, fla[9][9];
        file(a, 2);
        for (i = 0; i < 9; i++)
            for (j = 0; j < 9; j++)
            {
                if (a[i][j] == 0)
                    flag[i][j] = '0';
                else
                    flag[i][j] = '*';
            }
        sum = is_sudoku(a, fla);
        if (sum != 0)
        {
            cct_gotoxy(4, 1);
            draw(a, flag, fla);
            cct_gotoxy(0, 30);
            cout << "�����ļ���" << sum << "����ͻ�����޸���ȷ�����" << endl;
        }
        else
        {
            if (s == 0)
            {
                draw(a, flag, fla);
                cct_gotoxy(0, 31);
                cout << "[��ǰ����] : 1��a��";
                temporary(a[0][0], 6, 2);
            }
            if (choice == 1)
                back();
            while (1)
            {
                if (is_solution(a, fla) == 1)
                {
                    cct_gotoxy(0, 34);
                    cout << "����������Ϸ���!!!!!";
                    break;
                }
                if (s > 0)
                {
                    *nx = p[(s - 1) % _max].fx;
                    *ny = p[(s - 1) % _max].fy;
                    *nu = p[(s - 1) % _max].num;
                }
                cout << endl;
                t = _input(a, flag, steps, nx, ny, nu, fla, choice);
                if (t == 0 || t == 8 || t > 8)
                    draw(a, flag, fla);
                if (t > 8 || t == 8)
                {
                    if (t > 8 && t - 9 == 0 && a[*nx - 1][*ny - 'a'] == 0);
                    else
                        single(a[*nx - 1][*ny - 'a'], '0', 6 * (*ny - 'a') + 6, 3 * *nx - 1, '#', 2);
                }
                if (t != 0 && p != NULL && t < 9)
                {
                    p[s % _max].fx = *nx;
                    p[s % _max].fy = *ny;
                    p[s % _max].num = *nu;
                    s++;
                }
            }
        }
    }
    delete[]p;
}