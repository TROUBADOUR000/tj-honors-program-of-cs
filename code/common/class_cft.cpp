/* 2153592 ���ݷ� ��01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <io.h>
#include "../include/class_cft.h"
//�������������Ҫ��ͷ�ļ�
#include <sstream>
using namespace std;

#if ! ENABLE_LIB_TCFG_TOOLS	//����lib����ܱ���

/* ---------------------------------------------------------------
	 �������������Ҫ��struct/class/union/�����ȣ��ڲ������ã�
	 1����������Ϊ��̬��������
	 2����������ų���/������
	 3��������Ҫ��ȫ�ֱ��������һ��Ҫ�ã���static
   ---------------------------------------------------------------- */
static bool get_item_name(string line, string& name)
{
	int i, j;
	bool flag = 1, v = 0, end = 0;
	name = "";
	for (i = 0; line.c_str()[i] != '\0'; i++)
	{
		if (!v)
		{
			if (line.c_str()[i] == '#')
			{
				flag = 0;
				break;
			}
			if (line.c_str()[i] != ' ')
			{
				v = 1;
			}
		}
		if (v)
		{
			for (j = i;; j++)
				if (line.c_str()[j] == '=')
				{
					end = 1;
					break;
				}
				else if (line.c_str()[j] != ' ')
					break;
		}
		if (end)
			break;
		if (line.c_str()[i] == '[' || line.c_str()[i] == '\r' || line.c_str()[i] == '\n')
		{
			flag = 0;
			break;
		}
		if (v)
			name += line.c_str()[i];
	}
	name += "";
	return flag;
}

static bool get_item_value(string line, string name, string& value, bool enable_space = 0)
{
	int i;
	bool flag = 1, begin = 0, v = 0;
	value = "";
	for (i = 0; i < int(line.length()); i++)
	{
		if (line.c_str()[i] == '\r' || line.c_str()[i] == '\n')
			break;
		if (begin)
		{
			if (!v)
			{
				if (line.c_str()[i] == ' ')
					continue;
				else
					v = 1;
			}
			if(v)
			{
				if (!enable_space && line.c_str()[i] == ' ')
					break;
				value += line.c_str()[i];
			}
		}
		if (!begin && line.c_str()[i] == '=')
			begin = 1;
	}
	value += "";
	return flag;
}

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

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�
	 ���������
	 �� �� ֵ��
	 ˵    ����C++17����filesystem::resize_fileֱ�ӿ����޸��ļ���С��
			   ֮ǰ�İ汾�ޣ���˱���������C��ʽ��_chsize����
		   ��ʵ�֣���Ҫ��
	***************************************************************************/
int cft::file_resize(int newsize)
{
	msg.str("");
	/* �����С�Ǹ�����ֱ�ӷ���-1 */
	if (newsize < 0) {
		msg << "file_resize - invalid size(" << newsize << ")." << endl;
		return -1;
	}

	if (!fp.good())
		fp.clear();

	/* ȡfp��ǰָ��λ�� */
	int now_pos = int(fp.tellg());

	/* ���ȣ�C++��ʽ�ر� */
	fp.close();

	/* ��Σ�C��ʽ�򿪲��ı��ļ���С */
	FILE* fp_cstyle;
	fp_cstyle = fopen(filename.c_str(), "rb+"); //rw��ʽ��
	if (fp_cstyle) {
		if (_chsize(_fileno(fp_cstyle), newsize) < 0)
			msg << "file_resize - chsize failed" << endl; //����������Ϣ������ִ��(�ָ�C++��ʽ��)

		fclose(fp_cstyle); //�����Ƿ���ȷ��flcose
	}
	else
		msg << "file_resize - fopen (" << filename << ") failed" << endl;//����������Ϣ������ִ��(�ָ�C++��ʽ��)

	/* �ٴΣ�C++��ʽ���´򿪣��ܵ���resize��һ����rw�� */
	fp.open(filename, ios::in | ios::out | ios::binary);
	if (fp.is_open() == 0) {
		msg << "file_resize - reopen (" << filename << ") failed" << endl;//C++��ʽ�����򷵻�-1
		return -1;
	}

	/* �ļ�ָ���ƶ���ԭλ�� */
	fp.seekg(now_pos < newsize ? now_pos : newsize, ios::beg);

	return 0;
}

bool cft::get_group_name(string line, string& name)
{
	int i, j, flag = 0;
	bool omit = 0;
	name = "";
	for (i = 0; line.c_str()[i] != '\0'; i++)
	{
		if (flag > 0)
		{
			if (is_ignore_comment == CommentSwitch::on)
			{
				if (flag == 1)
				{
					if (line.c_str()[i] != ' ')
					{
						flag++;
						name += line.c_str()[i];
					}
				}
				else
				{
					if (line.c_str()[i] == ' ')
					{
						for (j = i + 1;; j++)
						{
							if (line.c_str()[j] == ']')
							{
								omit = 1;
								break;
							}
							if (line.c_str()[j] != ' ')
								break;
						}
					}
					if (line.c_str()[i] == ']')
						omit = 1;
					if (!omit)
						name += line.c_str()[i];
					else
						break;
				}
			}
			else
			{
				if (line.c_str()[i] == ']')
					break;
				name += line.c_str()[i];
			}
		}
		if (!flag && line.c_str()[i] == '#' || line.c_str()[i] == ';')
			break;
		if (!flag && line.c_str()[i] == '[')
			flag++;
	}
	name += "";
	return flag;
}

bool cft::is_group_existed(const char* group_name)
{
	string line, name;
	bool is_being = 0;
	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();
	while (getline(fp, line, '\n'))
	{
		if (get_group_name(line, name) && name == group_name)
		{
			is_being = 1;
			break;
		}
	}
	return is_being;
}

bool cft::is_simple_file()
{
	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();
	string line, name;
	bool flag = 1;
	while (getline(fp, line, '\n'))
	{
		if (get_group_name(line, name))
		{
			flag = 0;
			break;
		}
	}
	return flag;
}

int cft::num_of_item(const char* group_name, const char* item_name)
{
	bool Is_Simple_File = is_simple_file(), in_group = 0;
	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();
	string line, name;
	int num = 0;
	while (getline(fp, line, '\n'))
	{
		if (!Is_Simple_File)
		{
			if (get_group_name(line, name))
			{
				if (name == group_name)
					in_group = 1;
				else
					in_group = 0;
			}
			if (in_group && get_item_name(line, name))
			{
				if (name == item_name)
					num++;
			}
		}
		else
		{
			if (get_item_name(line, name) && name == item_name)
				num++;
		}
	}
	return num;
}

string cft::get_last_name()
{
	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();
	string line, na, name;
	while (getline(fp, line, '\n'))
	{
		if (get_group_name(line, na))
			name = na;
	}
	return name;
}

/* ---------------------------------------------------------------
	 �������class cft�г�Ա������ʵ��
	 ����ʵ�ֹ����в�Ҫ����cout�����Խ���Ϣ����msg�У���test�е���
---------------------------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
cft::cft()
{
	filename = "";
	is_ignore_comment = CommentSwitch::on;
	inspos = InsertPosition::First_line_after_group;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
cft::~cft()
{
	fp.close();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
cft::cft(const char* cfgname, int opt)
{
	filename = cfgname;
	if (opt == OPEN_OPT_RDONLY)
	{
		fp.open(filename, ios::binary | ios::in);
		if (!fp.is_open())
			exit(-1);
	}
	else
	{
		fp.open(filename, ios::binary | ios::in | ios::out | ios::app);
	}
	is_ignore_comment = CommentSwitch::on;
	inspos = InsertPosition::First_line_after_group;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
cft::cft(const string& cfgname, int opt)
{
	filename = cfgname;
	if (opt == OPEN_OPT_RDONLY)
	{
		fp.open(filename, ios::binary | ios::in);
		if (!fp.is_open())
			exit(-1);
	}
	else
	{
		fp.open(filename, ios::binary | ios::in | ios::out | ios::app);
	}
	is_ignore_comment = CommentSwitch::on;
	inspos = InsertPosition::First_line_after_group;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
void cft::open(const char* cfgname, int opt)
{
	filename = cfgname;
	if (opt == OPEN_OPT_RDONLY)
	{
		fp.open(filename, ios::binary | ios::in);
		if (!fp.is_open())
			exit(-1);
	}
	else
	{
		fp.open(filename, ios::binary | ios::in | ios::out | ios::app);
	}
	is_ignore_comment = CommentSwitch::on;
	inspos = InsertPosition::First_line_after_group;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
bool cft::is_open(void)
{
	return fp.is_open(); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
void cft::close()
{
	fp.close();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
void cft::print() const
{
	cout << msg.str() << endl;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����Ĭ��Ϊon
 ***************************************************************************/
void cft::set_comment_switch(CommentSwitch flag)
{
	is_ignore_comment = flag;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
CommentSwitch cft::get_comment_switch() const
{
	return this->is_ignore_comment;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����Ĭ��ΪFirst_line_after_group
 ***************************************************************************/
void cft::set_insert_position(InsertPosition pos)
{
	inspos = pos;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
InsertPosition cft::get_insert_position() const
{
	return this->inspos;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::group_add(const char* group_name)
{
	if (!is_open())
		return 0;

	fp.seekg(0, ios::beg);

	string name, line;
	while (getline(fp, line, '\n'))
	{
		if (get_group_name(line, name))
			if (name == group_name)
				return 0;
	}

	fp.clear();

	if (int(fp.tellg()) != 0)
		fp << endl;
	fp << '[' << group_name << ']' << endl;

	return 1; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::group_add(const string& group_name)
{
	return group_add(group_name.c_str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
  �� ɾ���ɹ�����n(ɾ���������)�����򷵻�0
  �� ���[test]�鲻���ڣ�ֱ�ӷ���0����
  �� ���[test]���ظ����ڣ����磺�ֹ��޸�ʹ����ͬ��������Ҫɾ������ͬ���鲢����2
 ***************************************************************************/
int cft::group_del(const char* group_name)
{
	int del_num = 0, i;
	bool del = 0, flag;

	if (!is_open())
		return 0;

	fp.seekg(0, ios::beg);
	fp.clear();

	string content = "", line, name;

	while (getline(fp, line, '\n'))
	{
		name = "";
		if (!del)
		{
			if (get_group_name(line, name) && name == group_name)
			{
				del_num++;
				del = 1;
			}
			else
			{
				content += line;
				content += "\n";
			}
		}
		else
		{
			flag = 1;
			for (i = 0; i < int(line.length()); i++)
			{
				if (line.c_str()[i] == '[')
					break;
				if (line.c_str()[i] != ' ' && line.c_str()[i] != '\t')
					flag = 0;
			}
			if (flag && get_group_name(line, name))
			{
				del = 0;
				content += line;
				content += "\n";
			}
		}
	}

	fp.clear();

	if (del_num > 0)
	{
		file_resize(int(content.length()));
		fp.clear();
		fp.seekp(0, ios::beg);
		fp << content;
	}

	return del_num; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
  �� ɾ���ɹ�����n(ɾ���������)�����򷵻�0
  �� ���[test]�鲻���ڣ�ֱ�ӷ���0����
  �� ���[test]���ظ����ڣ����磺�ֹ��޸�ʹ����ͬ��������Ҫɾ������ͬ���鲢����2
 ***************************************************************************/
int cft::group_del(const string& group_name)
{
	return group_del(group_name.c_str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_add(const char* group_name, const char* item_name, const int item_value)
{
	stringstream tmp;
	tmp << item_value;
	return item_add(group_name, item_name, tmp.str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_add(const char* group_name, const char* item_name, const double item_value)
{
	stringstream tmp;
	tmp << item_value;
	return item_add(group_name, item_name, tmp.str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_add(const char* group_name, const char* item_name, const char* item_value)
{
	string tmp;
	tmp = item_value;
	return item_add(group_name, item_name, tmp); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_add(const char* group_name, const char* item_name, const string& item_value)
{
	if (!is_open())
		return 0;

	string name, line, content = "";
	int times = 0;
	bool Is_Simple_File, is_existed = 0, is_added = 0;

	Is_Simple_File = is_simple_file();

	is_existed = bool(!!num_of_item(group_name, item_name));
	if (is_existed)
		return 0;

	if (!Is_Simple_File && group_name == NULL)
	{
		if (!Is_Simple_File || is_existed)
			return 0;

		fp.clear();
		fp.seekg(0, ios::end);

		fp << item_name << " = " << item_value << "\n";
		return 1;
	}

	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();

	if (Is_Simple_File)
	{
		if (inspos == InsertPosition::Begin_of_file)
		{
			content += item_name;
			content += " = ";
			content += item_value;
			content += "\n";
		}
		while (getline(fp, line, '\n'))
		{
			if (!is_added && inspos == InsertPosition::First_blank_line)
			{
				if (is_blank_line(line))
				{
					times++;
					if (times == 1)
					{
						content += item_name;
						content += " = ";
						content += item_value;
						content += "\n";
						is_added = 1;
					}
				}
			}
			content += line;
			content += "\n";
		}
		if (inspos == InsertPosition::End_of_file)
		{
			content += item_name;
			content += " = ";
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
						content += item_name;
						content += " = ";
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
								content += item_name;
								content += " = ";
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
						content += item_name;
						content += " = ";
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
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_add(const char* group_name, const char* item_name, const char item_value)
{
	stringstream tmp;
	tmp << item_value;
	return item_add(group_name, item_name, tmp.str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_add(const char* group_name, const char* item_name)
{
	string tmp = "";
	return item_add(group_name, item_name, tmp); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int cft::item_del(const char* group_name, const char* item_name)
{
	if (!is_open())
		return 0;

	int del_num;
	string content = "", line, name;
	bool Is_Simple_File = is_simple_file();

	del_num = num_of_item(group_name, item_name);

	if (!del_num)
		return 0;

	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();

	if (!Is_Simple_File)
	{
		while (getline(fp, line, '\n'))
		{
			if (get_group_name(line, name) && name == group_name)
			{
				content += line;
				content += "\n";
				while (getline(fp, line, '\n'))
				{
					if (get_item_name(line, name) && name == item_name)
						continue;
					content += line;
					content += "\n";
					if (get_group_name(line, name))
						break;
				}
			}
			else
			{
				content += line;
				content += "\n";
			}
		}
	}
	else
	{
		while (getline(fp, line, '\n'))
		{
			if (get_item_name(line, name) && name == item_name)
				continue;
			content += line;
			content += "\n";
		}
	}

	file_resize(int(content.length()));
	fp.clear();
	fp.seekp(0, ios::beg);
	fp << content;

	return del_num; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_update(const char* group_name, const char* item_name, const int item_value)
{
	stringstream tmp;
	tmp << item_value;
	return item_update(group_name, item_name, tmp.str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_update(const char* group_name, const char* item_name, const double item_value)
{
	stringstream tmp;
	tmp << item_value;
	return item_update(group_name, item_name, tmp.str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_update(const char* group_name, const char* item_name, const char* item_value)
{
	string tmp;
	tmp = item_value;
	return item_update(group_name, item_name, tmp); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_update(const char* group_name, const char* item_name, const string& item_value)
{
	if (!is_open())
		return 0;

	string content = "", line, name;
	bool Is_Simple_File = is_simple_file();

	if (!Is_Simple_File)
		if (!is_group_existed(group_name))
			return 0;

	if (!bool(!!num_of_item(group_name, item_name)))
		return item_add(group_name, item_name, item_value);
	else
	{
		fp.clear();
		fp.seekg(0, ios::beg);
		fp.clear();

		if (!Is_Simple_File)
		{
			while (getline(fp, line, '\n'))
			{
				if (get_group_name(line, name) && name == group_name)
				{
					content += line;
					content += "\n";
					while (getline(fp, line, '\n'))
					{
						if (get_item_name(line, name) && name == item_name)
						{
							content += item_name;
							content += " = ";
							content += item_value;
							content += '\n';
							continue;
						}
						content += line;
						content += "\n";
						if (get_group_name(line, name))
							break;
					}
				}
				else
				{
					content += line;
					content += "\n";
				}
			}
		}
		else
		{
			while (getline(fp, line, '\n'))
			{
				if (get_item_name(line, name) && name == item_name)
				{
					content += item_name;
					content += " = ";
					content += item_value;
					content += '\n';
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
	}

	return 1; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_update(const char* group_name, const char* item_name, const char item_value)
{
	stringstream tmp;
	tmp << item_value;
	return item_update(group_name, item_name, tmp.str()); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_update(const char* group_name, const char* item_name)
{
	string tmp = "";
	return item_update(group_name, item_name, tmp); //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_get_value(const char* group_name, const char* item_name, int& item_value)
{
	string tmp;
	stringstream a;
	int ret;
	ret = item_get_value(group_name, item_name, tmp);
	if (ret)
	{
		a << tmp;
		a >> item_value;
	}
	return ret; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_get_value(const char* group_name, const char* item_name, double& item_value)
{
	string tmp;
	stringstream a;
	int ret;
	ret = item_get_value(group_name, item_name, tmp);
	if (ret)
	{
		a << tmp;
		a >> item_value;
	}
	return ret; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_get_value(const char* group_name, const char* item_name, char* item_value, bool enable_space)
{
	string tmp;
	int ret, i;
	ret = item_get_value(group_name, item_name, tmp, enable_space);
	if (ret)
	{
		for (i = 0; i < int(tmp.length()); i++)
			item_value[i] = tmp.c_str()[i];
		item_value[i] = '\0';
	}
	return ret; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_get_value(const char* group_name, const char* item_name, string& item_value, bool enable_space)
{
	if (!item_get_value(group_name, item_name))
		return 0;

	string line, name;
	bool Is_Simple_File = is_simple_file();
	int ret = 0;

	fp.clear();
	fp.seekg(0, ios::beg);
	fp.clear();

	if (!Is_Simple_File)
	{
		while (getline(fp, line, '\n'))
		{
			if (get_group_name(line, name) && name == group_name)
			{
				while (getline(fp, line, '\n'))
				{
					if (get_item_name(line, name) && name == item_name)
					{
						ret = get_item_value(line, name, item_value, enable_space);
						break;
					}
				}
			}
		}
	}
	else
	{
		while (getline(fp, line, '\n'))
		{
			if (get_item_name(line, name))
			{
				if (name == item_name)
				{
					ret = get_item_value(line, name, item_value, enable_space);
					break;
				}
			}
		}
	}

	return ret; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_get_value(const char* group_name, const char* item_name, char& item_value)
{
	string tmp;
	stringstream a;
	int ret;
	ret = item_get_value(group_name, item_name, tmp);
	if (ret)
	{
		a << tmp;
		a >> item_value;
	}
	return ret; //�����޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int cft::item_get_value(const char* group_name, const char* item_name)
{
	if (!is_open())
		return 0;
	if (group_name)
		if (!is_group_existed(group_name))
			return 0;

	if (!bool(!!num_of_item(group_name, item_name)))
		return 0;

	return 1; //�����޸�
}

#endif  //end of ! ENABLE_LIB_TCFG_TOOLS

