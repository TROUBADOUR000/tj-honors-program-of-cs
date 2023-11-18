/* 2153592 ���ݷ� ��01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../include/class_aat.h"
//���б�Ҫ�����Լ�������ͷ�ļ�
using namespace std;

#if !ENABLE_LIB_AAT_TOOLS //��ʹ��lib����Ч

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */
#define INVALID_INT_VALUE_OF_SET	0x7FFFFFFF

static int char_to_int(const char* a)
{
	int n = 0, i;
	for (i = 0; a[i] != '\0'; i++)
	{
		n *= 10;
		if (a[i] >= '0' && a[i] <= '9')
			n += a[i] - '0';
		else
			return -1;
	}
	return n;
}

static bool IP_check(const string& t, unsigned& input)
{
	int i, flag = 0, tmp[4] = { 0 };
	input = 0;
	for (i = 0; i < int(t.length()); i++)
	{
		if (t.c_str()[i] == '.')
		{
		    if (tmp[flag] > 255 || tmp[flag] < 0)
				return 0;
			flag++;
		}
		else
		{
			tmp[flag] *= 10;
			tmp[flag] += t.c_str()[i] - '0';
		}
	}
	if (flag != 3 || t.c_str()[i - 1] == '.')
		return 0;
	for (i = 0; i < 4; i++)
	{
		input *= 256;
		input += tmp[i];
	}
	return 1;
}

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�
	 ���������
	 �� �� ֵ��
	 ˵    ����null
	***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	args_name = "";
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_bool_default = def;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_default��int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_int_default = def;
	extargs_int_min = _min;
	extargs_int_max = _max;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_set_default��int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	int _def, i, flag = 1;
	for (i = 0; i <= def_of_set_pos; i++)
		if (set[i] == INVALID_INT_VALUE_OF_SET)
		{
			flag = 0;
			break;
		}
	if (flag)
		_def = set[def_of_set_pos];
	else
		_def = set[0];
	extargs_int_default = _def;
	args_existed = 0;
	extargs_int_set = (int*)set;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str��ipaddr_with_default��ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_string_default = def;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str_with_set_default��str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	int i, flag = 1;
	string _def;
	for (i = 0; i <= def_of_set_pos; i++)
		if (set[i] == "")
		{
			flag = 0;
			break;
		}
	if (flag)
		_def = set[def_of_set_pos];
	else
		_def = set[0];
	extargs_string_default = _def;
	args_existed = 0;
	extargs_string_set = (string*)set;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{

}

/* ---------------------------------------------------------------
	 ����AAT���Զ����Ա������ʵ�֣�private��
   ---------------------------------------------------------------- */

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�
	 ���������
	 �� �� ֵ��
	 ˵    ������ʵ�֣���Ҫ��
	***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return args_name;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������!!��ֻ����0/1
			��ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!args_existed;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return extargs_int_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return extargs_string_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return extargs_ipaddr_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ extargs_ipaddr_value ��ֵ�� 0x7f000001 תΪ "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	string ip;
	int i, t[4];
	unsigned v = extargs_ipaddr_value;
	for (i = 0; i < 4; i++)
	{
		t[i] = v % 256;
		v /= 256;
	}
	stringstream out;
	out<< t[3] << "." << t[2] << '.' << t[1] << "." << t[0];
	out >> ip;
	return ip; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������follow_up_args���Ƿ��к�������
			./log_login --limit 10 --user root                    ���޺�����������0
			./ulimits_check --memory 256 --cpu 1 [./log_login ***]���к�����������1
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_process(const int argc, const char* const* const argv, args_analyse_tools* const args, const int follow_up_args)
{
	int i, j, tmp, flag, k, num = 1, follow_up = 0, category;
	string tmp_str;
	unsigned input;
	if (argc == 1)
		return 0;
	for (category = 1;; category++)
		if (args[category].args_name == "")
			break;
	for (i = 1; i < argc; i++)
	{
		flag = 0;
		if (!follow_up)
		{
			for (j = 0; j < category; j++)
			{
				if (!strcmp(argv[i], args[j].args_name.c_str()))
				{
					num++;
					if (args[j].args_existed)
					{
						cout << "����[" << args[j].args_name << "]�ظ�" << endl;
						return -1;
					}
					args[j].args_existed = 1;
					switch (args[j].extargs_type)
					{
					case ST_EXTARGS_TYPE::boolean:
						break;
					case ST_EXTARGS_TYPE::int_with_default:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:int, ��Χ[" << args[j].extargs_int_min
								<< ".." << args[j].extargs_int_max << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:int, ��Χ[" << args[j].extargs_int_min
								<< ".." << args[j].extargs_int_max << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
							return -1;
						}
						tmp = char_to_int(argv[++i]);
						if (tmp < 0)
						{
							cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�����������. (����:int, ��Χ["
								<< args[j].extargs_int_min << ".." << args[j].extargs_int_max << "] ȱʡ:"
								<< args[j].extargs_int_default << ")" << endl;
							return -1;
						}
						if (tmp > args[j].extargs_int_max || tmp < args[j].extargs_int_min)
							args[j].extargs_int_value = args[j].extargs_int_default;
						else
							args[j].extargs_int_value = tmp;
						break;
					case ST_EXTARGS_TYPE::int_with_error:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:int, ��Χ[" << args[j].extargs_int_min
								<< ".." << args[j].extargs_int_max << "])" << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:int, ��Χ[" << args[j].extargs_int_min
								<< ".." << args[j].extargs_int_max << "])" << endl;
							return -1;
						}
						tmp = char_to_int(argv[++i]);
						if (tmp < 0)
						{
							cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�����������. (����:int, ��Χ["
								<< args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
							return -1;
						}
						if (tmp > args[j].extargs_int_max || tmp < args[j].extargs_int_min)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ���ֵ(" << tmp << ")�Ƿ�. (����:int, ��Χ["
								<< args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
							return -1;
						}
						else
							args[j].extargs_int_value = tmp;
						break;
					case ST_EXTARGS_TYPE::int_with_set_default:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:int, ��ȡֵ[";
							for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (args[j].extargs_int_set[k + 1] != INVALID_INT_VALUE_OF_SET)
									cout << '/';
							}
							cout << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:int, ��ȡֵ[";
							for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (args[j].extargs_int_set[k + 1] != INVALID_INT_VALUE_OF_SET)
									cout << '/';
							}
							cout << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
							return -1;
						}
						tmp = char_to_int(argv[++i]);
						for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							if (tmp == args[j].extargs_int_set[k])
								break;
						if (args[j].extargs_int_set[k] == INVALID_INT_VALUE_OF_SET)
							flag = 1;
						if (flag)
							args[j].extargs_int_value = args[j].extargs_int_default;
						else if (tmp >= 0)
							args[j].extargs_int_value = tmp;
						else
						{
							cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�����������. (����:int, ��ȡֵ[";
							for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (args[j].extargs_int_set[k + 1] != INVALID_INT_VALUE_OF_SET)
									cout << '/';
							}
							cout << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
							return -1;
						}
						break;
					case ST_EXTARGS_TYPE::int_with_set_error:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:int, ��ȡֵ[";
							for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (args[j].extargs_int_set[k + 1] != INVALID_INT_VALUE_OF_SET)
									cout << '/';
							}
							cout << ']' << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:int, ��ȡֵ[";
							for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (args[j].extargs_int_set[k + 1] != INVALID_INT_VALUE_OF_SET)
									cout << '/';
							}
							cout << ']' << endl;
							return -1;
						}
						tmp = char_to_int(argv[++i]);
						for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							if (tmp == args[j].extargs_int_set[k])
								break;
						if (args[j].extargs_int_set[k] == INVALID_INT_VALUE_OF_SET)
							flag = 1;
						if (flag)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ���ֵ(" << tmp << ")�Ƿ�. (����:int, ��ȡֵ[";
							for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (args[j].extargs_int_set[k + 1] != INVALID_INT_VALUE_OF_SET)
									cout << '/';
							}
							cout << ']' << endl;
							return -1;
						}
						else if (tmp >= 0)
							args[j].extargs_int_value = tmp;
						else
						{
							cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�����������. (����:int, ��ȡֵ[";
							for (k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
							{
								cout << args[j].extargs_int_set[k];
								if (args[j].extargs_int_set[k + 1] != INVALID_INT_VALUE_OF_SET)
									cout << '/';
							}
							cout << ']' << endl;
							return -1;
						}
						break;
					case ST_EXTARGS_TYPE::str:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:string";
							if (args[j].extargs_string_default != "")
								cout << " ȱʡ:" << args[j].extargs_string_default;
							cout << ')' << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:string";
							if (args[j].extargs_string_default != "")
								cout << " ȱʡ:" << args[j].extargs_string_default;
							cout << ')' << endl;
							return -1;
						}
						args[j].extargs_string_value = argv[++i];
						break;
					case ST_EXTARGS_TYPE::str_with_set_default:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:string, ��ȡֵ[";
							for (k = 0; args[j].extargs_string_set[k] != ""; k++)
							{
								cout << args[j].extargs_string_set[k];
								if (args[j].extargs_string_set[k + 1] != "")
									cout << '/';
							}
							cout << "] ȱʡ:" << args[j].extargs_string_default << ")" << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:string, ��ȡֵ[";
							for (k = 0; args[j].extargs_string_set[k] != ""; k++)
							{
								cout << args[j].extargs_string_set[k];
								if (args[j].extargs_string_set[k + 1] != "")
									cout << '/';
							}
							cout << "] ȱʡ:" << args[j].extargs_string_default << ")" << endl;
							return -1;
						}
						tmp_str = argv[++i];
						for (k = 0; args[j].extargs_string_set[k] != ""; k++)
							if (tmp_str == args[j].extargs_string_set[k])
								break;
						if (args[j].extargs_string_set[k] == "")
							flag = 1;
						if (flag)
							args[j].extargs_string_value = args[j].extargs_string_default;
						else
							args[j].extargs_string_value = tmp_str;
						break;
					case ST_EXTARGS_TYPE::str_with_set_error:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:string, ��ȡֵ[";
							for (k = 0; args[j].extargs_string_set[k] != ""; k++)
							{
								cout << args[j].extargs_string_set[k];
								if (args[j].extargs_string_set[k + 1] != "")
									cout << '/';
							}
							cout << "]" << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:string, ��ȡֵ[";
							for (k = 0; args[j].extargs_string_set[k] != ""; k++)
							{
								cout << args[j].extargs_string_set[k];
								if (args[j].extargs_string_set[k + 1] != "")
									cout << '/';
							}
							cout << "]" << endl;
							return -1;
						}
						tmp_str = argv[++i];
						for (k = 0; args[j].extargs_string_set[k] != ""; k++)
							if (tmp_str == args[j].extargs_string_set[k])
								break;
						if (args[j].extargs_string_set[k] == "")
							flag = 1;
						if (flag)
							args[j].extargs_string_value = args[j].extargs_string_default;
						else
							args[j].extargs_string_value = tmp_str;
						break;
					case ST_EXTARGS_TYPE::ipaddr_with_default:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:IP��ַ ȱʡ:" << args[j].extargs_ipaddr_default << ")" << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:IP��ַ ȱʡ:" << args[j].extargs_ipaddr_default << ")" << endl;
							return -1;
						}
						tmp_str = argv[++i];
						if (IP_check(tmp_str, input))
							args[j].extargs_ipaddr_value = input;
						else
							args[j].extargs_ipaddr_value = args[j].extargs_ipaddr_default;
						break;
					case ST_EXTARGS_TYPE::ipaddr_with_error:
						if (argc == i + 1)
						{
							cout << "����" << args[j].args_name << "�ĸ��Ӳ�������(����:IP��ַ)" << endl;
							return -1;
						}
						if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-')
						{
							cout << "����" << args[j].args_name << "��ȱ�ٸ��Ӳ���(����:IP��ַ)" << endl;
							return -1;
						}
						tmp_str = argv[++i];
						if (IP_check(tmp_str, input))
							args[j].extargs_ipaddr_value = input;
						else
						{
							cout << "����[" << args[j].args_name << "]�ĸ��Ӳ���ֵ(" << tmp_str << ")�Ƿ�. (����:IP��ַ)" << endl;
							return -1;
						}
						break;
					}
					break;
				}
			}
			if (j == category)
			{
				if (!follow_up_args)
				{
					cout << "����[" << argv[i] << "]��ʽ�Ƿ�" << endl;
					return -1;
				}
				else
					follow_up = i;
			}
		}
		else
			break;
	}
	return num; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	cout << "name" << setw(12) << setfill(' ') << "type" << setw(25) << "default" <<
		setw(16) << "exists" << " value" << setw(21) << "range/set" << endl;
	cout << setw(112) << setfill('=') << '=' << endl;
	int i, j, n;
	unsigned name_length = 11, type_length = 20, default_length = 15;
	for (i = 0;; i++)
	{
		if (args[i].args_name == "")
			break;
		//if (args[i].args_name.length() > name_length)
			//name_length = args[i].args_name.length();
	}
	n = i;
	for (i = 0; i < n; i++)
	{
		cout << setiosflags(ios::left) << setfill(' ') << setw(name_length + 1) << args[i].args_name;
		switch (args[i].extargs_type)
		{
		case ST_EXTARGS_TYPE::boolean:
			cout << setw(type_length + 2) << "Bool";
			cout << setw(default_length + 2) << args[i].extargs_bool_default;
			cout << setw(7) << args[i].args_existed;
			if (args[i].args_existed)
				cout << setw(17) << "true";
			else
				cout << setw(17) << '/';
			cout << '/';
			break;
		case ST_EXTARGS_TYPE::int_with_default:
			cout << setw(type_length + 2) << "IntWithDefault";
			cout << setw(default_length + 2) << args[i].extargs_int_default;
			cout << setw(7) << args[i].args_existed;
			if (args[i].args_existed)
				cout << setw(17) << args[i].extargs_int_value;
			else
				cout << setw(17) << '/';
			cout << '[' << args[i].extargs_int_min << ".." << args[i].extargs_int_max << ']';
			break;
		case ST_EXTARGS_TYPE::int_with_set_default:
			cout << setw(type_length + 2) << "IntWithDefault";
			cout << setw(default_length + 2) << args[i].extargs_int_default;
			cout << setw(7) << args[i].args_existed;
			if (args[i].args_existed)
				cout << setw(17) << args[i].extargs_int_value;
			else
				cout << setw(17) << '/';
			for (j = 0;; j++)
			{
				if (args[i].extargs_int_set[j] == INVALID_INT_VALUE_OF_SET)
					break;
				else if (j > 0)
					cout << '/';
				cout << args[i].extargs_int_set[j];
			}
			break;
		case ST_EXTARGS_TYPE::int_with_error:
			cout << setw(type_length + 2) << "IntWithError";
			cout << setw(default_length + 2) << '/';
			cout << setw(7) << args[i].args_existed;
			if (args[i].args_existed)
				cout << setw(17) << args[i].extargs_int_value;
			else
				cout << setw(17) << '/';
			cout << '[' << args[i].extargs_int_min << ".." << args[i].extargs_int_max << ']';
			break;
		case ST_EXTARGS_TYPE::int_with_set_error:
			cout << setw(type_length + 2) << "IntSETWithError";
			cout << setw(default_length + 2) << '/';
			cout << setw(7) << args[i].args_existed;
			if (args[i].args_existed)
				cout << setw(17) << args[i].extargs_int_value;
			else
				cout << setw(17) << '/';
			for (j = 0;; j++)
			{
				if (args[i].extargs_int_set[j] == INVALID_INT_VALUE_OF_SET)
					break;
				else if (j > 0)
					cout << '/';
				cout << args[i].extargs_int_set[j];
			}
			break;
		case ST_EXTARGS_TYPE::str:
			cout << setw(type_length + 2) << "String";
			if (args[i].extargs_string_default == "")
				cout << setw(default_length + 2) << '/';
			else
				cout << setw(default_length + 2) << args[i].extargs_string_default;
			cout << setw(7) << args[i].args_existed;
			cout << setw(17) << (args[i].args_existed == 1 ? args[i].extargs_string_value : "/");
			cout << '/';
			break;
		case ST_EXTARGS_TYPE::str_with_set_default:
			cout << setw(type_length + 2) << "StringSETWithDefault";
			cout << setw(default_length + 2) << args[i].extargs_string_default;
			cout << setw(7) << args[i].args_existed;
			cout << setw(17) << (args[i].args_existed == 1 ? args[i].extargs_string_value : "/");
			for (j = 0;; j++)
			{
				if (args[i].extargs_string_set[j] == "")
					break;
				else if (j > 0)
					cout << '/';
				cout << args[i].extargs_string_set[j];
			}
			break;
		case ST_EXTARGS_TYPE::str_with_set_error:
			cout << setw(type_length + 2) << "StringSETWithError";
			cout << setw(default_length + 2) << '/';
			cout << setw(7) << args[i].args_existed;
			cout << setw(17) << (args[i].args_existed == 1 ? args[i].extargs_string_value : "/");
			for (j = 0;; j++)
			{
				if (args[i].extargs_string_set[j] == "")
					break;
				else if (j > 0)
					cout << '/';
				cout << args[i].extargs_string_set[j];
			}
			break;
		case ST_EXTARGS_TYPE::ipaddr_with_error:
			cout << setw(type_length + 2) << "IPAddrWithDefault";
			cout << setw(default_length + 2) << '/';
			cout << setw(7) << args[i].args_existed;
			cout << setw(17) << (args[i].args_existed == 1 ? args[i].get_str_ipaddr() : "/");
			cout << '/';
			break;
		case ST_EXTARGS_TYPE::ipaddr_with_default:
			cout << setw(type_length + 2) << "IPAddrWithError";
			cout << setw(default_length + 2) << args[i].extargs_string_default;
			cout << setw(7) << args[i].args_existed;
			cout << setw(17) << (args[i].args_existed == 1 ? args[i].get_str_ipaddr() : "/");
			cout << '/';
			break;	
		}
		cout << endl;
	}
	cout << endl;
	return 0; //�˾������Ҫ�޸�
}

#endif // !ENABLE_LIB_AAT_TOOLS
