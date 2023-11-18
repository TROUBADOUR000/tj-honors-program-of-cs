/* 2153592 ���ݷ� ��01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <io.h>
#include <sstream>
using namespace std;

#include "../include/class_aat.h"

static int char_to_int(char* tmp)
{
	int i, n = 0;
	for (i = 0; tmp[i] != '\0'; i++)
	{
		n *= 10;
		n += tmp[i];
	}
	return n;
}

struct _info
{
	char course[9] = { '\0' };
	char stu[10] = { '\0' };
	char name[32] = { '\0' };
	bool is_right1;
	bool is_right2;
	int num;
	char no[25][10] = { '\0' };
	char na[25][32] = { '\0' };
	bool is_mutual[25];
	int score;
};

static void sort(_info* stu, int num)
{
    struct _info tmp;
    int i, j;
    for (i = 0; i < num - 1; i++)
        for (j = i + 1; j < num; j++)
        {
            if (char_to_int(stu[i].course) > char_to_int(stu[j].course))
            {
                tmp = stu[i];
                stu[i] = stu[j];
                stu[j] = tmp;
            }
        }
    for (i = 0; i < num - 1; i++)
        for (j = i + 1; j < num; j++)
        {
            if (char_to_int(stu[i].course) == char_to_int(stu[j].course))
            {
                if (char_to_int(stu[i].stu) > char_to_int(stu[j].stu))
                {
                    tmp = stu[i];
                    stu[i] = stu[j];
                    stu[j] = tmp;
                }
            }
            else
                break;
        }
}

static void _sort(_info& stu)
{
    int i, j;
    char tmp[64];
    for (i = 0; i < stu.num - 1; i++)
        for (j = i + 1; j < stu.num; j++)
        {
            if (char_to_int(stu.no[i]) > char_to_int(stu.no[j]))
            {
                strcpy(tmp, stu.no[i]);
                strcpy(stu.no[i], stu.no[j]);
                strcpy(stu.no[j], tmp);
                strcpy(tmp, stu.na[i]);
                strcpy(stu.na[i], stu.na[j]);
                strcpy(stu.na[j], tmp);
            }
        }
}

static void read_stulist(ostringstream& msg, string stulist_fname, _info* stu, int& num, string debug)
{
    int i, j, flag = 0, l = 0, length, is_error = 0;
    char name[25], ch;
    sprintf(name, "./source/%s", stulist_fname.c_str());
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
                        stu[num].stu[j] = ch;
                    else if (flag == 2)
                        stu[num].name[j] = ch;
                    else if (flag == 3)
                        stu[num].is_right1 = !!(ch - '0');
                    else if (flag == 4)
                        stu[num].is_right2 = !!(ch - '0');
                    j++;
                }
            }
            if (is_error == 1)
                break;
            if (flag != 4)
                msg << "\t--> ѧ������[" << name << "]�еĵ�[" << l << "]���д�����" << endl;
            else
            {
                for (i = 0; i < num; i++)
                    if (!strcmp(stu[num].stu, stu[i].stu))
                    {
                        msg << "\t--> ѧ������[" << name << "]�еĵ�[" << l << "]��ѧ��[" << stu[num].stu << "]�ظ�������" << endl;
                        break;
                    }
                if (i == num)
                    num++;
            }
        }
        else if (in.peek() == '#')
        {
            ch = '\0';
            while (ch != '\n')
                ch = in.get();
            msg << "\t--> ѧ������[" << name << "]�еĵ�[" << l << "]��Ϊע���У�����" << endl;
        }
        else
        {
            in.get();
            if (in.peek() == '\n')
                in.get();
        }
    }
    sort(stu, num);
    msg << "��ȡ��ɣ���[" << l << "]�������ݣ�ȥ�غ�ѧ������[" << num << "]��";
    if (debug.c_str()[1] + debug.c_str()[2] + debug.c_str()[3] + debug.c_str()[4] > '0' * 4)
        msg << endl;
    in.close();
}

static void print_stulist(ostringstream& msg, _info* stu, int num, string debug)
{
    int i;
    msg << "���" << '\t' << "�κ�" << '\t' << "ѧ��" << '\t' << "����" << '\t' << "ԭʼ������ȷ" << '\t' << "У�������ȷ" << endl;
    for (i = 0; i < num; i++)
    {
        msg << (i + 1) << '#' << '\t' << stu[i].course << '\t' << stu[i].stu << '\t' <<
            stu[i].name << '\t' << stu[i].is_right1 << '\t' << stu[i].is_right2;
        if (i != num - 1 || debug.c_str()[2] + debug.c_str()[3] + debug.c_str()[4] > '0' * 3)
            msg << endl;
    }
}

static void read_2nd_line(ostringstream& msg, string filename, _info* stu, int num, string debug)
{
    int i, j, k = 0, flag, suicide, l = 0, n;
    char name[100], ch = '\0', old_ch;
    bool is_right[25] = { 0 }, alph;
    char no[25][64], na[25][64];
    msg << "���" << '\t' << "�κ�" << '\t' << "ѧ��" << '\t' << "����" << '\t' << "�ļ���" << '\t' << "�������" << endl;
    for (i = 0; i < num; i++)
    {
        flag = 1;
        j = 0;
        suicide = 0;
        k = 0;
        l = 0;
        alph = 0;
        msg << i + 1 << '#' << '\t' << stu[i].course << '\t' << stu[i].stu << '\t' << stu[i].name << '\t' << filename << '\t';
        sprintf(name, "./source/%s-%s/%s-%s-%s", stu[i].course, filename.c_str(), stu[i].stu, stu[i].name, filename.c_str());
        stu[i].num = 0;
        ifstream in;
        in.open(name, ios::in);
        if (!in.is_open())
        {
            msg << "δ�ύ" << endl;
            stu[i].is_right2 = 0;
        }
        else
        {
            while (1)
            {
                ch = in.get();
                if (ch != '\n' && ch != '\r' && ch != ' ' && ch != '\t')
                    flag = 0;
                if (!flag && (ch == '\n' || ch == '\r'))
                    break;
            }
            flag = 1;
            while (1)
            {
                old_ch = ch;
                ch = in.get();
                l++;
                if (l > 512)
                {
                    flag = 5;
                    stu[i].is_right2 = 0;
                    break;
                }
                if (flag == 1)
                {
                    if (ch == '/')
                    {
                        ch = in.get();
                        if (ch == '/')
                        {
                            flag = 2;
                            continue;
                        }
                        else if (ch == '*')
                        {
                            flag = 3;
                            continue;
                        }
                        else
                        {
                            flag = 0;
                            break;
                        }
                    }
                    else if (ch != '\n' && ch != '\r' && ch != '\t' && ch != ' ')
                    {
                        flag = 0;
                        break;
                    }
                }
                else
                {
                    if (k == 63)
                    {
                        j++;
                        k = 0;
                    }
                    if (ch == ' ' || ch == '\t')
                    {
                        if (k > 0)
                        {
                            j++;
                            k = 0;
                        }
                    }
                    else if (ch == '\n' || ch == '\r')
                    {
                        if (flag == 2)
                        {
                            if (old_ch == ' ' || old_ch == '\t')
                                j--;
                            break;
                        }
                        else
                        {
                            flag = 0;
                            break;
                        }
                    }
                    else if (ch == '*')
                    {
                        ch = in.peek();
                        if (ch == '/')
                        {
                            in.get();
                            if (flag == 3)
                            {
                                if (j > 0 && (old_ch == ' ' || old_ch == '\t'))
                                    j--;
                            }
                            else
                                flag = 0;
                            break;
                        }
                        else
                        {
                            if (j % 2 == 1)
                            {
                                na[j / 2][k] = '*';
                                na[j / 2][k + 1] = '\0';
                            }
                            else
                            {
                                no[j / 2][k] = '*';
                                no[j / 2][k + 1] = '\0';
                            }
                            k++;
                            alph = 1;
                        }
                    }
                    else
                    {
                        if (j % 2 == 1)
                        {
                            na[j / 2][k] = ch;
                            na[j / 2][k + 1] = '\0';
                        }
                        else
                        {
                            no[j / 2][k] = ch;
                            no[j / 2][k + 1] = '\0';
                        }
                        k++;
                        alph = 1;
                    }
                }
            }
            if (flag >= 1 && flag <= 4)
            {
                n = (j + 1) / 2;
                int s, t;
                for (k = 0; k < n && !suicide; k++)
                {
                    t = 0;
                    is_right[k] = 1;
                    if (no[k][0] < '0' || no[k][0]>'9' || strlen(no[k]) != 7)
                    {
                        if (flag == 4)
                            msg << "\t\t\t\t\t";
                        msg << "��[" << k + 1 << "]���ѧ��[" << no[k] << "]/����[" << na[k] << "]����ȷ������" << endl;
                        is_right[k] = 0;
                        flag = 4;
                        continue;
                    }
                    for (s = 0; s < num; s++)
                    {
                        if (!strcmp(no[k], stu[s].stu))
                        {
                            if (s == i)
                            {
                                if (flag == 4)
                                    msg << "\t\t\t\t\t";
                                msg << "��[" << k + 1 << "]��д���Լ���ˣ����ô��" << endl;
                                is_right[k] = 0;
                                suicide = (k > 0 ? k : -1);
                                flag = 4;
                                break;
                            }
                            if (s != i && strcmp(na[k], stu[s].name))
                            {
                                if (flag == 4)
                                    msg << "\t\t\t\t\t";
                                msg << "��[" << k + 1 << "]���ѧ��[" << no[k] << "]/����[" << na[k] << "]����ȷ������" << endl;
                                is_right[k] = 0;
                                flag = 4;
                                break;
                            }
                        }
                        else
                            t++;
                    }
                    if (t == num)
                    {
                        if (flag == 4)
                            msg << "\t\t\t\t\t";
                        msg << "��[" << k + 1 << "]���ѧ��[" << no[k] << "]/����[" << na[k] << "]����ȷ������" << endl;
                        is_right[k] = 0;
                        flag = 4;
                    }
                }
                if (alph && j % 2 == 0)
                {
                    if (flag == 4)
                        msg << "\t\t\t\t\t";
                    msg << "��[" << k << "]��ѧ���������Ϣ��ȫ������" << endl;
                    flag = 4;
                }
                for (j = 0; j < (suicide != 0 ? suicide : n); j++)
                {
                    if (is_right[j])
                    {
                        for (k = 0; k < stu[i].num; k++)
                        {
                            if (char_to_int(no[j]) == char_to_int(no[k]))
                            {
                                msg << "�����б��[" << j + 1 << "]���ѧ��[" << no[j] << "]�ظ�������" << endl;
                                flag = 4;
                                break;
                            }
                        }
                        if (k == stu[i].num)
                        {
                            stu[i].num++;
                            strcpy(stu[i].na[stu[i].num - 1], na[j]);
                            strcpy(stu[i].no[stu[i].num - 1], no[j]);
                        }
                    }
                }
                if (flag == 2 || flag == 3)
                    msg << "ok" << endl;
                _sort(stu[i]);
                if (stu[i].num)
                {
                    msg << "\t��������ȷ������" << endl;
                    k = 1;
                    for (j = 0; j < stu[i].num; j++)
                        msg << '\t' << k++ << '\t' << stu[i].no[j] << '\t' << stu[i].na[j] << endl;
                }
            }
            else if (!flag)
                msg << "��2����Ϣ�д�[����//��/*��ͷ��������/*��ͷ��*/����ͬһ��]" << endl;
            else
                msg << "�ǿ��г����˺�����[512]�ֽ�." << endl;
            in.close();
        }
    }
    if (debug.c_str()[3] + debug.c_str()[4] > 2 * '0')
        msg << endl;
}

static void print_2nd_line(ostringstream& msg, _info* stu, int num, string debug)
{
    const char result[3][15] = { "ok","��������","����Чƥ����" };
    int i, j, k, l;
    msg << "���" << '\t' << "�κ�" << '\t' << "ѧ��" << '\t' << "����" << endl;
    for (i = 0; i < num; i++)
    {
        msg << i + 1 << '#' << '\t' << stu[i].course << '\t' << stu[i].stu << '\t' << stu[i].name;
        if (!stu[i].num)
        {
            msg << '\t' << result[2];
            if (i + 1 < num || debug.c_str()[4] > '0')
                msg << endl;
        }
        else
        {
            msg << endl;
            for (j = 0; j < stu[i].num; j++)
            {
                stu[i].is_mutual[j] = 0;
                msg << '\t' << "$" << j + 1 << '\t' << stu[i].no[j] << '\t' << stu[i].na[j] << '\t';
                for (k = 0; k < num; k++)
                    if (!strcmp(stu[k].stu, stu[i].no[j]))
                    {
                        for (l = 0; l < stu[k].num; l++)
                        {
                            if (!strcmp(stu[k].no[l], stu[i].stu))
                            {
                                stu[i].is_mutual[j] = 1;
                                break;
                            }
                        }
                        break;
                    }
                if (stu[i].is_mutual[j])
                    msg << result[0];
                else
                    msg << result[1];
                if ((i + 1 < num || j + 1 < stu[i].num) || debug.c_str()[4] > '0')
                    msg << endl;
            }
        }
    }
}

static void print_finalscore(ostringstream& msg, _info* stu, int num)
{
    msg << "���" << '\t' << "�κ�" << '\t' << "ѧ��" << '\t' << "����" << '\t' << "ԭʼ����÷�" << '\t'
        << "������ԭʼ��" << '\t' << "������۷�1" << '\t' << "������۷�2" << '\t' << "�������ܷ�" << endl;
    int i, j, k, l, deduction[2];
    for (i = 0; i < num; i++)
    {
        deduction[0] = 0;
        deduction[1] = 0;
        stu[i].score = (stu[i].is_right2 ? 100 : 0);
        msg << i + 1 << '#' << '\t' << stu[i].course << '\t' << stu[i].stu << '\t' << stu[i].name << '\t';
        msg << (stu[i].is_right1 ? 100 : 0) << '\t' << stu[i].score << '\t';
        for (j = 0; j < stu[i].num; j++)
            deduction[0] -= 100 * (1 - stu[i].is_mutual[j]);
        msg << deduction[0] << '\t';
        if (stu[i].is_right2)
        {
            for (j = 0; j < stu[i].num; j++)
            {
                for (k = 0; k < num; k++)
                {
                    if (!strcmp(stu[k].stu, stu[i].no[j]))
                    {
                        for (l = 0; l < num; l++)
                        {
                            if (!strcmp(stu[i].no[j], stu[l].stu))
                            {
                                if (!stu[l].is_right1)
                                    deduction[1] -= 20;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            if (stu[i].num < 5)
                deduction[1] -= 20 * (5 - stu[i].num);
        }
        else
            deduction[1] = -100;
        msg << deduction[1] << '\t';
        stu[i].score += deduction[0] + deduction[1];
        msg << stu[i].score;
        if (i + 1 < num)
            msg << endl;
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ���������ԣ�ʵ��ʹ��ʱ��������һ��cpp(����hw_check_secondline.cpp)������
 ***************************************************************************/
int check_secondline(const string& cno, const string& stulist_fname, const string& filename, const string& debug)
{
    cout << "��ʼ����Դ�����ļ����м��..." << endl;
    
    string de = debug;
    if (debug == "")
        de = "11111";
    ostringstream msg;
    _info* stu = NULL;
    stu = new(nothrow)_info[1024];
    int num = 0;
    /* ���κ�.dat */
    msg << "1.��ȡ�κ������ļ�" << endl;
    read_stulist(msg, stulist_fname, stu, num, de);
    if (de.c_str()[0] == '1')
        cout << msg.str() << endl;

    msg.str(""); //���msg�е���Ϣ
    msg << "2.���κ�+ѧ�������ѧ������" << endl;
    print_stulist(msg, stu, num, de);
    if (de.c_str()[1] == '1')
        cout << msg.str() << endl;

    msg.str(""); //���msg�е���Ϣ
    msg << "3.����ѧ����ȡ��2�еĽ�����Ϣ" << endl;
    read_2nd_line(msg, filename, stu, num, de);
    if (de.c_str()[2] == '1')
        cout << msg.str();

    /* ��˫��������Ϣƥ�� */
    msg.str(""); //���msg�е���Ϣ
    msg << "4.����ѧ���ĵ�2�л���ƥ����" << endl;
    print_2nd_line(msg, stu, num, de);
    if (de.c_str()[3] == '1')
        cout << msg.str() << endl;

    /* �ٸ���ͨ�������м�� */
    msg.str(""); //���msg�е���Ϣ
    msg << "5.����ѧ���ĵ�2�л������յ÷�" << endl;
    print_finalscore(msg, stu, num);
    if (de.c_str()[4] == '1')
        cout << msg.str() << endl;

    delete[]stu;
    return 0;
}

struct info
{
	char num[8];
	char student[10];
	char fullname[25];
	char abbreviation[10];
	int is_being;
};

static void print(string filename, info* stu, int is_right[], int num)
{
	int i, j, is[3], t;
	char tip[3][12] = { "ѧ�Ų�ƥ�� ","������ƥ�� ","�༶��ƥ�� " };
	for (i = 0; i < num; i++)
	{
		cout << stu[i].num << '-' << stu[i].student << '-' << stu[i].fullname << " : " << filename << " : ";
		if (!stu[i].is_being)
			cout << "δ�ύ" << endl;
		else
		{
			switch (is_right[i])
			{
			case 0:
				cout << "δ�ύ" << endl;
				break;
			case 1:
				cout << "δȡ��������Ϣ" << endl;
				break;
			case 3:
				cout << "������Ϣ��ȫ" << endl;
				break;
			case 4:
				cout << "������Ϣ����" << endl;
				break;
			case 5:
				cout << "ѧ�Ų�ƥ�� " << endl;
				break;
			default:
				t = is_right[i] / 8;
				for (j = 0; j < 3; j++)
				{
					is[2 - j] = t % 2;
					t /= 2;
				}
				for (j = 0; j < 3; j++)
				{
					if (!is[j])
						cout << tip[j];
				}
				if (is[0] + is[1] + is[2] == 3)
					cout << "ͨ��";
				cout << endl;
			}
		}
	}
}

static void classify(fstream& in, info stu, int* is_right)
{
	char ch = '\0', sdat[3][25], old_ch;
	int flag = 1, i = 0, j = 0, na = 0, nu = 0, cl = 0;
	while (1)
	{
		old_ch = ch;
		ch = in.get();
		if (flag == 1)
		{
			if (ch == '/')
			{
				ch = in.get();
				if (ch == '/')
				{
					flag = 3;
				}
				else if (ch == '*')
				{
					flag = 2;
				}
				else
				{
					*is_right = 1;
					break;
				}
			}
			else if (ch != ' ' && ch != '\n' && ch != (char)(9) && ch != '\r')
			{
				*is_right = 1;
				break;
			}
		}
		else
		{
			if (ch == '\n' || ch == '\r')
			{
				if (old_ch != ' ' && old_ch != (char)(9))
					i++;
				*is_right = flag == 3 ? -1 : 1;
				break;
			}
			else if (ch == '*')
			{
				if (old_ch != ' ' && old_ch != (char)(9))
					i++;
				if (flag == 2 && (ch = in.get()) == '/')
				{
					*is_right = -1;
					break;
				}
				else
				{
					*is_right = 1;
					break;
				}
			}
			else if (ch == ' ' || ch == (char)(9))
			{
				if (j > 0)
				{
					i++;
					j = 0;
				}
			}
			else
			{
				if (j < 25 && i < 3)
				{
					sdat[i][j++] = ch;
					sdat[i][j] = '\0';
				}
				if (i >= 3)
					j++;
			}
		}
	}
	if (*is_right == -1)
	{
		if (i == 3)
		{
			*is_right = 2;
		}
		else if (i > 3)
		{
			*is_right = 4;
		}
		else
			*is_right = 3;
	}
	if (*is_right == 2)
	{
		for (i = 0; i < 3; i++)
		{
			if (!nu)
				if (!strcmp(sdat[i], stu.num))
					nu = 1;
			if (!na)
				if (!strcmp(sdat[i], stu.student))
					na = 1;
			if (!cl)
				if (!strcmp(sdat[i], stu.fullname))
					cl = 1;
			if (!cl)
				if (!strcmp(sdat[i], stu.abbreviation))
					cl = 1;
			if (nu && na && cl)
				break;
		}
		*is_right = (nu * 4 + na * 2 + cl) * 8;
		*is_right = *is_right == 0 ? 64 : *is_right;
	}
}

static void check(string filename, char* folder_path, int* is_right, info stu)
{
	fstream in;
    char name[100];
    sprintf(name, "./source/%s/%s", folder_path, filename.c_str());
	*is_right = 56;                                //������111000,000����,3��1�ֱ��ʾѧ�š��������༶����ȷ��,ѧ����ȷ����find�б�֤
	in.open(name, ios::in);
	if (!in.is_open())
	{
		*is_right = 0;
	}
	else
	{
		classify(in, stu, is_right);
		in.close();
	}
}

static void find(const char* course_num, string filename, int is_right[], info* stu, int n)
{
	int i, len = 1 + strlen(course_num);
    char f[25], fn[10];
	int k;
    sprintf(f, "./source/%s-*", course_num);
	struct _finddata_t file;
	long HANDLE;
	k = HANDLE = _findfirst(f, &file);
	for (i = 0; i < n; i++)
		is_right[i] = 5;
	while (k != -1)
	{
		for (i = 0; file.name[i + len] != '\0'; i++)
			fn[i] = file.name[i + len];
		fn[i] = '\0';
		for (i = 0; i < n; i++)
		{
			if (!strcmp(stu[i].num, fn))
			{
				stu[i].is_being = 1;
				check(filename, file.name, &is_right[i], stu[i]);
				break;
			}
		}
		k = _findnext(HANDLE, &file);
	}
	_findclose(HANDLE);
}

static void sort(info* stu, int num)
{
	struct info tmp;
	int i, j;
	for (i = 0; i < num - 1; i++)
		for (j = i + 1; j < num; j++)
		{
			if (char_to_int(stu[i].num) > char_to_int(stu[j].num))
			{
				tmp = stu[i];
				stu[i] = stu[j];
				stu[j] = tmp;
			}
		}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ���������ԣ�ʵ��ʹ��ʱ��������һ��cpp(����hw_check_secondline.cpp)������
 ***************************************************************************/
int check_firstline(const string& cno, const string& stilist_fname, const string& filename)
{
    cout << "��ʼ����Դ�����ļ����м��..." << endl;

    fstream in;
    char ch, file[50], temp[1024];
    struct info* stu = NULL;
    int num = 0, i, j;
    sprintf(file, "./source/%s", stilist_fname.c_str());
    in.open(file, ios::in);
    if (!in.is_open())
    {
        printf("�޷���\n");
        return 0;
    }
    while (!in.eof())
    {
        ch = in.get();
        if (ch == '\n')
            num++;
    }
    stu = new(nothrow)info[num];
    if (stu == NULL)
    {
        printf("����ʧ�ܣ�\n");
        in.close();
        return -1;
    }
    in.close();
    in.open(file, ios::in);
    if (!in.is_open())
    {
        printf("�޷���\n");
        return 0;
    }
    for (i = 0; i < num; i++)
    {
        in.getline(temp, 1024, '\n');
        istringstream msg;
        msg.str(temp);
        msg >> stu[i].num >> stu[i].student >> stu[i].fullname >> stu[i].abbreviation;
        for (j = 0; j < i; j++)
            if (!strcmp(stu[i].num, stu[j].num))
            {
                i--;
                break;
            }
        stu[i].is_being = 0;
    }
    in.close();
    sort(stu, num);
    int* is_right;
    is_right = new(nothrow)int[num];
    find(cno.c_str(), filename, is_right, stu, num);
    print(filename, stu, is_right, num);
    delete[]is_right;
    delete[]stu;

    return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static void usage(const char* const procname, const int args_num)
{
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;

	cout << endl;
	cout << "Usage: " << procname << " { --firstline | --secondline  }" << endl;
	cout << setw(wkey) << ' ' << "{ --courseid cid }" << endl;
	cout << setw(wkey) << ' ' << "{ --filename name/all }" << endl;
	cout << setw(wkey) << ' ' << "{ --stulist_fname name }" << endl;
	cout << setw(wkey) << ' ' << "{ --debug 11111 }" << endl;
	cout << endl;

	cout << setw(wkey) << ' ' << "��ѡ�ָ���������(��ѡһ)" << endl;
	cout << setw(wopt) << ' ' << "--firstline     : ���м��" << endl;
	cout << setw(wopt) << ' ' << "--fileformat    : ���м��" << endl;
	cout << setw(wkey) << ' ' << "��ѡ�" << endl;
	cout << setw(wopt) << ' ' << "--courseid      : �κ�" << endl;
	cout << setw(wopt) << ' ' << "--filename      : �ļ���(all��ʾȫ��)" << endl;
	cout << setw(wopt) << ' ' << "--stulist_fname : ��Ӧ��ѧ���б��ļ���" << endl;
	cout << setw(wkey) << ' ' << "��ѡ�" << endl;
	cout << setw(wopt) << ' ' << "--debug         : Debugѡ��(��������--secondline��Ĭ��11111)" << endl;
	cout << endl;

	cout << "e.g.   " << procname << " --firstline --courseid 100692 --filename 3-b1.cpp --stulist_fname 100692.txt               : ���100692�γ̵�3-b1.cpp������" << endl;
	cout << "       " << procname << " --firstline --courseid 100692 --filename all --stulist_fname 100692-firstline.txt          : ���100692�γ̵�ȫ���ļ�������" << endl;
	cout << "       " << procname << " --secondline --courseid 100742 --filename 15-b5.c --stulist_fname 100692.dat               : ���100742�γ̵�15-b5.c�Ĵ��У����ȫ����Ϣ" << endl;
	cout << "       " << procname << " --secondline --courseid 100742 --filename 15-b5.c --stulist_fname 100692.dat --debug 01001 : ���100742�γ̵�15-b5.c�Ĵ��У������2��5����Ϣ" << endl;
	cout << endl;

	if (args_num == 1) {
		cout << endl << endl << "����cmd�¼Ӳ�������." << endl << endl;
		system("pause");
	}
}

/* enum��˳��Ҫ��args������˳�򱣳�һ��
   ����enum class ֻ�ǲ���ÿ�ζ���ǿ��ת�� */
enum OPT_ARGS {
	OPT_ARGS_HELP = 0, OPT_ARGS_FIRSTLINE, OPT_ARGS_SECONDLINE, OPT_ARGS_COURSEID, OPT_ARGS_FILENAME, OPT_ARGS_STULIST_FNAME, OPT_ARGS_DEBUG
};

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char** argv)
{
	args_analyse_tools args[] = {
		args_analyse_tools("--help", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--firstline", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--secondline", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--courseid", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--filename", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--stulist_fname", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--debug", ST_EXTARGS_TYPE::str, 1, string("11111")),
		args_analyse_tools()  //���һ�������ڽ���
	};

	int cur_argc, ret = 0;

	//���һ������1����ʾ��ѡ������⣬������������
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//������Ϣ�ں������Ѵ�ӡ
		return -1;
	}

	/* ��Ҫ�Լ�ʵ�� */
	args_analyse_print(args);

	/* ��help�����⴦�� */
	if (args[OPT_ARGS_HELP].existed()) {
		//ֻҪ�� --help���������������ԣ���ʾ��������
		usage(argv[0], argc);
		return -1; //ִ�����ֱ���˳�
	}

	/* ������������ */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_SECONDLINE].existed() != 1) {
		usage(argv[0], argc);
		cout << "����[" << args[OPT_ARGS_FIRSTLINE].get_name() << "|" << args[OPT_ARGS_SECONDLINE].get_name() << "]����ָ��һ����ֻ��ָ��һ��" << endl;
		return -1;
	}

	/* �жϱ�ѡ���Ƿ���� */
	if (args[OPT_ARGS_COURSEID].existed() == 0) {
		cout << "����ָ������[" << args[OPT_ARGS_COURSEID].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �жϱ�ѡ���Ƿ���� */
	if (args[OPT_ARGS_FILENAME].existed() == 0) {
		cout << "����ָ������[" << args[OPT_ARGS_FILENAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �жϱ�ѡ���Ƿ���� */
	if (args[OPT_ARGS_STULIST_FNAME].existed() == 0) {
		cout << "����ָ������[" << args[OPT_ARGS_STULIST_FNAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �ж� --firstline �� --debug �Ƿ��ͻ */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_DEBUG].existed() == 2) {
		cout << "����[" << args[OPT_ARGS_FIRSTLINE].get_name() << "]����Ҫ[" << args[OPT_ARGS_DEBUG].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �ж� --secondline �� --filename all �Ƿ��ͻ */
	if (args[OPT_ARGS_SECONDLINE].existed()) {
		if (args[OPT_ARGS_FILENAME].get_string() == "all") {
			cout << "����[" << args[OPT_ARGS_SECONDLINE].get_name() << "]��[" << args[OPT_ARGS_SECONDLINE].get_name() << "]����Ϊall" << endl;
			usage(argv[0], argc);
			return -1;
		}
	}

	if (args[OPT_ARGS_FIRSTLINE].existed()) {
		/* �������м�� */
		check_firstline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string());
	}
	else if (args[OPT_ARGS_SECONDLINE].existed()) {
		/* ���д��м�� */
		check_secondline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string(), args[OPT_ARGS_DEBUG].get_string());
	}

	return 0;
}
