/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include "../include/class_cft.h"
#include "../include/class_cft_gai.h"
#include "../include/cmd_console_tools.h"
#include "../include/class_aat.h"
#include "90-02-b7.h"
using namespace std;

#define NUM 8

void relevant_set(const args_analyse_tools* args, int& row, int& col, int& high, int& width, string& typeface)
{
	if (args[2].existed())
		row = args[2].get_int();
	if (args[3].existed())
		col = args[3].get_int();
	if (args[4].existed())
		typeface = args[4].get_string();
	if (args[5].existed())
		high = args[5].get_int();
	if (args[6].existed())
		if (args[6].get_int() == -1)
			width = typeface == "新宋体" ? 0 : 8;
		else
			width = args[6].get_int();
}

void read_cfg_file(cft_gai& fcfg, int co[8])
{
	int tmp, i;
	const string name[NUM] = {
		"keyword_bkcolor",
		"keyword_fgcolor",
        "datatype_bkcolor",
        "datatype_fgcolor",
        "notes_bkcolor",
        "notes_fgcolor",
        "normal_bkcolor",
        "normal_fgcolor"
	};
	const int set_color[8] = {
		COLOR_BLACK,
		COLOR_HYELLOW,
		COLOR_BLACK,
		COLOR_HPINK,
		COLOR_BLACK,
		COLOR_GREEN,
		COLOR_BLACK,
		COLOR_HWHITE
	};
	for (i = 0; i < NUM; i++)
	{
		if (fcfg.item_get_value("color", name[i].c_str(), tmp) && tmp >= 0 && tmp <= 14)
			co[i] = tmp;
		else
			co[i] = set_color[i];
	}
}

string* read_keyword(cft_gai& fcfg)
{
	return fcfg.item_get_all("keyword");
}

string* read_datatype(cft_gai& fcfg)
{
	return fcfg.item_get_all("datatype");
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：const char* filename ：要打开的源程序文件名
			const int row        ：窗口的函数
		   const int col        ：窗口的列数
		   const string typeface ：字体名
		   const int high       ：字体高度
		   const int width      ：字体宽度
  返 回 值：
  说    明：
 ***************************************************************************/
void show(const char* filename, const int row, const int col, const string typeface, const int high, const int width, const color word_color[4], string* keywords, string* datatypes)
{
	cct_cls();
	cct_setconsoleborder(col, row, col * 2, row * 10);
	cct_setfontsize(typeface.c_str(), high, width);
	fstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}

	enum types { keyword = 0, datatype, note, normal };
	int i, j, k, type, is_note = 0, is_in, l = 0;
	string tmp;
	char ch;

	while (!fin.eof())
	{	
		tmp = "";
		
		while ((ch = fin.get()) != EOF)
		{
			tmp += ch;
			if (ch == '\r' || ch == '\n' || ch == '\t' || ch == ' ')
			{
				if (is_note == 1)
					is_note = 0;
				break;
			}
		}

		for (j = 0; j < int(tmp.length()); j++)
		{
			type = types::normal;
			is_in = 0;
			if (j + 1 < int(tmp.length()) && tmp.c_str()[j] == '/' && tmp.c_str()[j + 1] == '/')
				is_note = 1;
			else if (j + 1 < int(tmp.length()) && tmp.c_str()[j] == '/' && tmp.c_str()[j + 1] == '*')
				is_note = 2;
			else if (tmp.c_str()[j] == '\n')
				if (is_note == 1)
					is_note = 0;

			if (is_note > 0)
				type = types::note;
			else
			{
				for (i = 0; keywords[i] != ""; i++)
				{
					if (j + keywords[i].length() < tmp.length())
					{
						for (k = 0; k < int(keywords[i].length()); k++)
							if (tmp[j + k] != keywords[i][k])
								break;
						if (k == keywords[i].length())
						{
							is_in = 1;
							if (j > 0)
								if ((tmp[j - 1] > 'a' && tmp[j - 1] < 'z') || (tmp[j - 1] > 'A' && tmp[j - 1] < 'Z'))
									is_in = 0;
							if (j + k + 1 < int(tmp.length()))
								if ((tmp[j + k + 1] > 'a' && tmp[j + k + 1] < 'z') || (tmp[j + k + 1] > 'A' && tmp[j + k + 1] < 'Z'))
									is_in = 0;
							if (is_in)
							{
								type = types::keyword;
								l = keywords[i].length();
							}
						}
					}
				}
				if (type == types::normal)
				{
					for (i = 0; datatypes[i] != ""; i++)
					{
						if (j + datatypes[i].length() < tmp.length())
						{
							for (k = 0; k < int(datatypes[i].length()); k++)
								if (tmp[j + k] != datatypes[i][k])
									break;
							if (k == datatypes[i].length())
							{
								is_in = 1;
								if (j > 0)
									if ((tmp[j - 1] > 'a' && tmp[j - 1] < 'z') || (tmp[j - 1] > 'A' && tmp[j - 1] < 'Z'))
										is_in = 0;
								if (j + k + 1 < int(tmp.length()))
									if ((tmp[j + k + 1] > 'a' && tmp[j + k + 1] < 'z') || (tmp[j + k + 1] > 'A' && tmp[j + k + 1] < 'Z'))
										is_in = 0;
								if (is_in)
								{
									type = types::datatype;
									l = datatypes[i].length();
								}
							}
						}
					}
				}
			}

			cct_setcolor(word_color[type].bgcolor, word_color[type].fgcolor);
			
			if (j + 1 < int(tmp.length()) && tmp.c_str()[j] == '*' && tmp.c_str()[j + 1] == '/')
			{
				cout << tmp.c_str()[j++];
				is_note = 0;
			}
			
			if (is_in)
			{
				for (k = 0; k < l; k++)
					cout << tmp.c_str()[j++];
				j--;
			}
			else
				cout << tmp.c_str()[j];

			cct_setcolor(word_color[3].bgcolor, word_color[3].fgcolor);
		}
	}

	fin.close();
}

//90-02-b7.exe --src_filename example.cpp --cfg_filename color.conf