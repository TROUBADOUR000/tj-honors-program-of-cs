/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <io.h>
#include "../include/class_cft.h"
#include "../include/class_cft_gai.h"
//允许加入其它需要的头文件
using namespace std;

#if ! ENABLE_LIB_TCFG_TOOLS	//禁用lib后才能编译

/* ---------------------------------------------------------------
	 允许加入其它需要的struct/class/union/函数等（内部工具用）
	 1、函数必须为静态，不对外
	 2、允许定义符号常量/常变量
	 3、尽量不要用全局变量，如果一定要用，加static
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
    下面给出class cft_gai中成员函数的实现
	整个实现过程中不要出现cout，可以将信息放入msg中，在test中调用
---------------------------------------------------------------- */

/***************************************************************************
函数名称：
功    能：与class ctf不同
输入参数：
返 回 值：
说    明：
***************************************************************************/
int cft_gai::item_add(const char* group_name, const char* item_value)
{
	string tmp = item_value;
	return item_add(group_name, tmp); //按需修改
}

/***************************************************************************
  函数名称：
  功    能：与class ctf不同
  输入参数：
  返 回 值：
  说    明：
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
					/* 下一组组名 */
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

	return 1; //按需修改
}

/***************************************************************************
  函数名称：
  功    能：与class ctf不同
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int cft_gai::item_del(const char* group_name, const char* item_value)
{
	string tmp = item_value;
	return item_del(group_name, tmp); //按需修改
}

/***************************************************************************
  函数名称：
  功    能：与class ctf不同
  输入参数：
  返 回 值：
  说    明：
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

	return ret; //按需修改
}

/***************************************************************************
  函数名称：
  功    能：取得某group下的全部有效行（空行、仅含空格/tab的行等都排除）
  输入参数：
  返 回 值：动态申请一个string数组，每个元素存一行，返回首地址即可
  说    明：按需申请，不要浪费，调用出记得释放
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

	return info; //按需修改
}

#endif
