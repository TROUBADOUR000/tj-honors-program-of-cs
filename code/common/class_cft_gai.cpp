/* 2153592 ���ݷ� ��01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <io.h>
#include "../include/class_cft.h"
#include "../include/class_cft_gai.h"
//�������������Ҫ��ͷ�ļ�
using namespace std;

#if ! ENABLE_LIB_TCFG_TOOLS	//����lib����ܱ���

/* ---------------------------------------------------------------
	 �������������Ҫ��struct/class/union/�����ȣ��ڲ������ã�
	 1����������Ϊ��̬��������
	 2����������ų���/������
	 3��������Ҫ��ȫ�ֱ��������һ��Ҫ�ã���static
   ---------------------------------------------------------------- */
static bool is_blank_line(string line)
{
	int i;
	bool ret = 1;
	for (i = 0; i < int(line.length()); i++)
	{
		if (line.c_str()[i] != ' ' && line.c_str()[i] != '\r' && line.c_str()[i] != '\n')
		{
			ret = 0;
			break;
		}
	}
	return ret;
}

static string removal(string t, bool enable_annotation = 0)
{
	int i, j, flag = 0;
	bool out = 0;
	string tmp = "";
	for (i = 0; i < int(t.length()); i++)
	{
		if (enable_annotation)
			if (t.c_str()[i] == '#' || t.c_str()[i] == ';')
				break;
		if (!flag && t.c_str()[i] != ' ')
			flag = 1;
		if (flag)
		{
			tmp += t.c_str()[i];
			for (j = i + 1; j < int(t.length()); j++)
			{
				if (enable_annotation)
				{
					if (t.c_str()[j] == '#' || t.c_str()[j] == ';')
					{
						flag = 2;
						break;
					}
				}
				if (t.c_str()[j] == ' ' || t.c_str()[j] == '\t'|| t.c_str()[j] == '\r'|| t.c_str()[j] == '\n')
				{
					flag = 0;
				}
				else
				{
					flag = 1;
					break;
				}
			}
			if (j == t.length() || flag == 2)
				out = 1;
			if (out)
				break;
		}
	}
	tmp += "";

	return tmp;
}

/* ---------------------------------------------------------------
    �������class cft_gai�г�Ա������ʵ��
	����ʵ�ֹ����в�Ҫ����cout�����Խ���Ϣ����msg�У���test�е���
---------------------------------------------------------------- */

/***************************************************************************
�������ƣ�
��    �ܣ���class ctf��ͬ
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int cft_gai::item_add(const char* group_name, const char* item_value)
{
	string tmp = item_value;
	return item_add(group_name, tmp); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���class ctf��ͬ
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft_gai::item_add(const char* group_name, const string& item_value)
{
	if (!is_open())
		return 0;

	string name, line, content = "";
	int times = 0;
	bool Is_Simple_File, is_added = 0, in_group = 0;

	Is_Simple_File = is_simple_file();

	if (group_name == NULL)
	{
		if (!Is_Simple_File)
			return 0;

		fp.clear();
		fp.seekg(0, ios::end);

		fp << item_value << "\n";
		return 1;
	}

	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();

	if (Is_Simple_File)
	{
		if (inspos == InsertPosition::Begin_of_file)
		{
			content += item_value;
			content += "\n";
		}
		while (getline(fp, line, '\n'))
		{
			if (in_group && !is_added && inspos == InsertPosition::First_blank_line)
			{
				if (is_blank_line(line))
				{
					times++;
					if (times == 1)
					{
						content += item_value;
						content += "\n";
						is_added = 1;
					}
				}
			}
			if (get_group_name(line, name))
				if (name == group_name)
					in_group = 1;
				else
					in_group = 0;
			content += line;
			content += "\n";
		}
		if (inspos == InsertPosition::End_of_file)
		{
			content += item_value;
			content += "\n";
		}
	}
	else
	{
		bool Is_Last = (group_name == get_last_name());

		fp.clear();
		fp.seekg(0, ios::beg);
		fp.clear();
		
		while (getline(fp, line, '\n'))
		{
			if (get_group_name(line, name) && name == group_name)
			{
				times++;
				if (times == 1)
				{
					content += line;
					content += "\n";
					if (!is_added && inspos == InsertPosition::First_line_after_group)
					{
						content += item_value;
						content += "\n";
						is_added = 1;
					}
					if (fp.peek() == EOF)
						if (!is_added && inspos == InsertPosition::First_blank_line_after_group)
						{
							content += item_value;
							content += "\n";
							is_added = 1;
						}
					while (getline(fp, line, '\n'))
					{
						if (!is_added && inspos == InsertPosition::First_blank_line_after_group)
						{
							if (is_blank_line(line))
							{
								content += item_value;
								content += "\n";
								is_added = 1;
							}
						}
						if (get_group_name(line, name))
						{
							times++;
							break;
						}
						content += line;
						content += "\n";
					}
					if (!is_added && inspos == InsertPosition::Before_next_group)
					{
						content += item_value;
						content += "\n";
						is_added = 1;
					}
					/* ��һ������ */
					if (!Is_Last)
					{
						content += line;
						content += "\n";
					}
				}
				else
				{
					content += line;
					content += "\n";
				}
			}
			else
			{
				content += line;
				content += "\n";
			}
		}
	}

	file_resize(int(content.length()));
	fp.clear();
	fp.seekp(0, ios::beg);
	fp << content;

	return 1; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���class ctf��ͬ
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft_gai::item_del(const char* group_name, const char* item_value)
{
	string tmp = item_value;
	return item_del(group_name, tmp); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���class ctf��ͬ
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft_gai::item_del(const char* group_name, const string& item_value)
{
	if (!is_open())
		return 0;

	string content = "", line, name, value, _line;
	bool Is_Simple_File = is_simple_file(), in_group = 0;
	int ret = 0;


	value = removal(item_value, 1);

	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();

	if (!Is_Simple_File)
	{
		while (getline(fp, line, '\n'))
		{
			if (!ret && in_group)
			{
				_line = removal(line, (is_ignore_comment == CommentSwitch::on));
				if (_line == value)
				{
					ret = 1;
					continue;
				}
			}
			if (get_group_name(line, name))
				if (name == group_name)
					in_group = 1;
				else
					in_group = 0;			
			content += line;
			content += "\n";
		}
	}
	else
	{
		while (getline(fp, line, '\n'))
		{
			_line = removal(line, (is_ignore_comment == CommentSwitch::on));
			if (_line == value)
			{
				ret = 1;
				continue;
			}
			content += line;
			content += "\n";
		}
	}

	file_resize(int(content.length()));
	fp.clear();
	fp.seekp(0, ios::beg);
	fp << content;

	return ret; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡ��ĳgroup�µ�ȫ����Ч�У����С������ո�/tab���еȶ��ų���
  ���������
  �� �� ֵ����̬����һ��string���飬ÿ��Ԫ�ش�һ�У������׵�ַ����
  ˵    �����������룬��Ҫ�˷ѣ����ó��ǵ��ͷ�
 ***************************************************************************/
string* cft_gai::item_get_all(const char* group_name)
{
	int num_valid = 0, n = 1;

	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();

	string* info, line, _line, name;
	int i, j = 0;
	bool flag, in_group = 0;

	while (getline(fp, line, '\n'))
	{
		if (get_group_name(line, name))
		{
			if (name == group_name)
			{
				in_group = 1;
				continue;
			}
			else
				in_group = 0;
		}
		if (in_group)
		{
			flag = 0;
			for (i = 0; i < int(line.length()); i++)
				if (line.c_str()[i] != ' ' && line.c_str()[i] != '\t' && line.c_str()[i] != '\r' && line.c_str()[i] != '\n')
				{
					flag = 1;
					break;
				}
			if (flag)
				num_valid++;
		}
	}

	info = new(nothrow)string[num_valid + 1];
	if (info == NULL)
		return NULL;

	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();

	while (getline(fp, line, '\n'))
	{
		if (get_group_name(line, name))
		{
			if (name == group_name)
			{
				in_group = 1;
				continue;
			}
			else
				in_group = 0;
		}
		if (in_group)
		{
			_line = removal(line, (is_ignore_comment == CommentSwitch::on));
			flag = 0;
			for (i = 0; i < int(_line.length()); i++)
				if (_line.c_str()[i] != ' ' && _line.c_str()[i] != '\t' && _line.c_str()[i] != '\r' && _line.c_str()[i] != '\n')
				{
					flag = 1;
					break;
				}
			if (flag && _line.length() > 0)
			{
				info[j] = _line;
				j++;
			}
		}
		if (j == num_valid)
			break;
	}

	info[num_valid] = "";

	return info; //�����޸�
}

#endif
