/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/class_cft.h"
#include "../include/class_cft_gai.h"
#include "../include/class_aat.h"
#include "90-02-b5-fk.h"
using namespace std;

int main(int argc, char** argv)
{
	args_analyse_tools args[] = {
		args_analyse_tools("--cfgfile", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--checkgname", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--courseid", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--stufile", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--maxerror", ST_EXTARGS_TYPE::int_with_default, 1, 15, 0, 999),
		args_analyse_tools("--minerror", ST_EXTARGS_TYPE::int_with_default,1, 3, 0, 999),
		args_analyse_tools()  //最后一个，用于结束
	};

	if (args_analyse_process(argc, argv, args, 0) < 0)
		return 0;

	int maxerror, minerror, num = 0, i;
	string group_name, courseid, stufile, * forbidden_words = NULL;
	group_name = args[1].get_string();
	courseid = args[2].get_string();
	stufile = args[3].get_string();

	cft_gai fcfg;
	fcfg.open(args[0].get_string().c_str());
	if (!fcfg.is_open())
	{
		cout << "文件打开失败" << endl;
		return 0;
	}
	forbidden_words = cft_read_file(fcfg, group_name, maxerror, minerror, forbidden_words);
	fcfg.close();

	info* stu = NULL;
	stu = new(nothrow)info[1024];
	if (stu == NULL)
		return 0;
	read_stulist(courseid, stu, num);

	for (i = 0; i < num; i++)
		check_fribidden_keywords(fcfg, courseid, stu[i], stufile, maxerror, minerror, forbidden_words, group_name == "check_1");

	print_list(stu, num);

	delete[]forbidden_words;
	delete[]stu;

	return 0;
}