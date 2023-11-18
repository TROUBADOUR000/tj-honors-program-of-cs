/* 2153592 ºúÒÝ·² ÐÅ01 */
#pragma once
#include "../include/class_cft_gai.h"

struct info
{
	char course[9] = { '\0' };
	char stu_no[8] = { '\0' };
	char stu_name[30] = { '\0' };
	int num;
};

string* cft_read_file(cft_gai& fcfg, const string group_name, int& maxerror, int& minerror, string* forbidden_words);

void read_stulist(string courseid, info* stu, int& num);

void check_fribidden_keywords(cft_gai& fcfg, string courseid, info& stu, string stufile, const int maxerror, const int minerror, const string* const forbidden_words, bool is_all);

void print_list(info* stu, const int num);