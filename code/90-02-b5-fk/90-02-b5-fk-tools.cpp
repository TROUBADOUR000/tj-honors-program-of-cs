/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include "90-02-b5-fk.h"
using namespace std;

string* cft_read_file(cft_gai& fcfg, const string group_name, int& maxerror, int& minerror, string* forbidden_words)
{
    int tmp;
	const string name = "系统设置";
	fcfg.set_comment_switch(CommentSwitch::on);

	if (fcfg.item_get_value(name.c_str(), "最大错误数量", tmp) && tmp >= 0 && tmp <= 999)
		maxerror = tmp;
	else
		maxerror = 0;
	if (fcfg.item_get_value(name.c_str(), "最小错误数量", tmp) && tmp >= 0 && tmp <= maxerror)
		minerror = tmp;
	else
		minerror = 0;

	return fcfg.item_get_all(group_name.c_str());
}

int char_to_int(char* tmp)
{
    int i, n = 0;
    for (i = 0; tmp[i] != '\0'; i++)
    {
        n *= 10;
        n += tmp[i];
    }
    return n;
}

void sort(info* stu, const int num)
{
    info tmp;
    int i, j;
    for (i = 0; i < num - 1; i++)
        for (j = i + 1; j < num; j++)
        {
            if (char_to_int(stu[i].stu_no) > char_to_int(stu[j].stu_no))
            {
                tmp = stu[i];
                stu[i] = stu[j];
                stu[j] = tmp;
            }
        }
}

void read_stulist(string courseid, info* stu, int& num)
{
    int i, j, flag = 0, l = 0, length, is_error = 0;
    char name[25], ch;
    sprintf(name, "./source/%s.dat", courseid.c_str());

    fstream in;
    in.open(name, ios::in);
    if (!in.is_open())
        exit(-1);
    while (in.peek() != EOF)
    {
        l++;
        length = 0;
        if (in.peek() != '#' && in.peek() != '\n' && in.peek() != '\r')
        {
            j = 0;
            flag = 0;
            while ((ch = in.get()) != '\n')
            {
                length++;
                if (length > 256)
                {
                    is_error = 1;
                    l--;
                    break;
                }
                if (ch == '\t')
                {
                    flag++;
                    j = 0;
                }
                else
                {
                    if (flag == 0)
                        stu[num].course[j] = ch;
                    else if (flag == 1)
                        stu[num].stu_no[j] = ch;
                    else if (flag == 2)
                        stu[num].stu_name[j] = ch;
                    j++;
                }
            }
            if (is_error == 1)
                break;
            if (flag == 3)
            {
                for (i = 0; i < num; i++)
                    if (!strcmp(stu[num].stu_no, stu[i].stu_no))
                        break;
                if (i == num)
                    num++;
            }
        }
        else if (in.peek() == '#')
        {
            ch = '\0';
            while (ch != '\n')
                ch = in.get();
        }
        else
        {
            in.get();
            if (in.peek() == '\n')
                in.get();
        }
    }
    sort(stu, num);
    in.close();
}

void check(const string* forbidden_words, int& errnum, fstream& in, const int maxerror)
{
    string tmp;
    int i;
    bool flag = 0;
    while (!in.eof())
    {
        tmp = "";
        in >> tmp;
        for (i = 0; forbidden_words[i] != ""; i++)
        {
            if (tmp == forbidden_words[i])
                errnum++;
            if (maxerror && errnum == maxerror)
            {
                flag = 1;
                break;
            }
        }
        if (flag)
            break;
    }
}

void check_fribidden_keywords(cft_gai& fcfg, string courseid, info& stu, string stufile, const int maxerror, const int minerror, const string* const forbidden_words, bool is_all)
{
    int i, errnum = 0;
    string tmp;
    char name[100];
    sprintf(name, "./source/%s-%s/%s-%s-%s", stu.course, stufile.c_str(), stu.stu_no, stu.stu_name, stufile.c_str());
    fstream in;
    in.open(name, ios::in);
    if (!in.is_open())
    {
        stu.num = -1;
        return;
    }

    if (is_all)
    {
        string* forbid;
        for (i = 0; forbidden_words[i] != ""; i++)
        {
            forbid = fcfg.item_get_all(forbidden_words[i].c_str());
            check(forbid, errnum, in, maxerror);
            delete[]forbid;
        }
    }
    else
        check(forbidden_words, errnum, in, maxerror);

    if (errnum <= minerror)
        stu.num = 0;
    else
        stu.num = errnum - minerror;

    in.close();
}

void print_list(info* stu, const int num)
{
    int i;
    for (i = 0; i < num; i++)
    {
        cout << stu[i].course << '-' << stu[i].stu_no << '-' << stu[i].stu_name << " : ";
        switch (stu[i].num)
        {
        case -1:
            cout << "未提交" << endl;
            break;
        case 0:
            cout << "OK" << endl;
            break;
        default:
            cout << stu[i].num << "项违规" << endl;
            break;
        }
    }
}