/* 2153592 ºúÒÝ·² ÐÅ01 */
#pragma once

struct color {
	string type;
	int fgcolor;
	int bgcolor;
};

void relevant_set(const args_analyse_tools* args, int& row, int& col, int& high, int& width, string& typeface);

void read_cfg_file(cft_gai& fcfg, int co[8]);

string* read_keyword(cft_gai& fcfg);

string* read_datatype(cft_gai& fcfg);

void show(const char* filename, const int row, const int col, const string typeface, const int high,
	const int width, const color word_color[4], string* keywords, string* datatypes);