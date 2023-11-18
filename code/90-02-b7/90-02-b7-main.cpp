/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/class_cft.h"
#include "../include/class_cft_gai.h"
#include "../include/cmd_console_tools.h"
#include "../include/class_aat.h"
#include "90-02-b7.h"
using namespace std;

int main(int argc, char** argv)
{
	string type[2] = { "新宋体","点阵字体" };
	args_analyse_tools args[] = {
		args_analyse_tools("--src_filename", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--cfg_filename", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--row", ST_EXTARGS_TYPE::int_with_default, 1, 30, 20, 50),
		args_analyse_tools("--col", ST_EXTARGS_TYPE::int_with_default,1, 120, 50, 200),
		args_analyse_tools("--font", ST_EXTARGS_TYPE::str_with_set_default, 1, 0, type),
		args_analyse_tools("--high", ST_EXTARGS_TYPE::int_with_default, 1, 16, 0, 20),
		args_analyse_tools("--width", ST_EXTARGS_TYPE::int_with_default,1, -1, 0, 20),
		args_analyse_tools()  //最后一个，用于结束
	};

	if (args_analyse_process(argc, argv, args, 0) < 0)
		return 0;

	int row = 30, col = 120, high = 16, width = 8;
	string typeface = "新宋体";

	relevant_set(args, row, col, high, width, typeface);

	int co[8];
	string* keywords, * datatypes;

	cft_gai fcfg;
	fcfg.open(args[1].get_string().c_str());
	if (!fcfg.is_open())
	{
		cout << "文件打开失败" << endl;
		return -1;
	}
	read_cfg_file(fcfg, co);
	keywords = read_keyword(fcfg);
	datatypes = read_datatype(fcfg);
	fcfg.close();

	const color word_color[] = {
		{"keyword",co[1],co[0]},
		{"datatype",co[3],co[2]},
		{"notes",co[5],co[4]},
		{"normal",co[7],co[6]},
	};

	show(args[0].get_string().c_str(), row, col, typeface, high, width, word_color, keywords, datatypes);

	delete[]keywords;
	delete[]datatypes;

	return 0;
}
