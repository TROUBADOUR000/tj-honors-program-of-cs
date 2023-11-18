/* 2153592 胡逸凡 信01 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
//可根据需要再加入其它需要的头文件
#include "../include/class_aat.h"
using namespace std;

/* 运行自行添加其它函数，也可以将自定义函数放在其它cpp中 */

#define NUM 16

static int usage(const char* const procname)
{
	cout << "Usage: " << procname << " [-l 大小] [-n 数量] [-t] IP地址" << endl;
	cout << "       " << setw(35) << setfill('=') << '=' << endl;
	cout << "        参数  附加参数 ";
	cout << setfill(' ') << setiosflags(ios::left) << setw(9) << "范围" << "默认值" << endl;
	cout << "       " << setw(35) << setfill('=') << '=' << endl;
	cout << "        -l    1        [32..64000] 64" << endl;
	cout << "        -n    1        [1..1024]   4" << endl;
	cout << "        -t    0        [0..1]      0" << endl;
	cout << "       " << setw(35) << setfill('=') << '=' << endl;
	return 0;
}

enum OPT_ARGS {
	OPT_ARGS_L = 0, OPT_ARGS_N, OPT_ARGS_T, OPT_ARGS_IP
};

int main(int argc, char* argv[])
{
	int cur_argc, is_err = 0;

	args_analyse_tools args[] = {
		args_analyse_tools("--l",    ST_EXTARGS_TYPE::int_with_default,        1, 64, 32, 64000),
		args_analyse_tools("--n",    ST_EXTARGS_TYPE::int_with_default,          1, 4, 1, 1024),
		args_analyse_tools("--t",      ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools()  //最后一个，用于结束
	};

	if (argc == 1)
	{
		usage(argv[0]);
		return -1;
	}

	if ((cur_argc = args_analyse_process(argc, argv, args, 1)) < 0) {
		//错误信息在函数中已打印
		return -1;
	}

	//判断是否输入非法参数并读取额外参数IP
	if (argc - cur_argc > 1)
		is_err = 1;
	else if (argc - cur_argc == 0)
		is_err = 2;
	else
	{
		int i, flag = 0, tmp = 0;
		for (i = 0; i < int(strlen(argv[cur_argc])); i++)
		{
			if (argv[cur_argc][i] == '.')
			{
				if (tmp > 255 || tmp < 0)
				{
					is_err = 2;
					break;
				}
				flag++;
				tmp = 0;
			}
			else if (argv[cur_argc][i] >= '0' && argv[cur_argc][i] <= '9')
			{
				tmp *= 10;
				tmp += argv[cur_argc][i] - '0';
			}
			else
			{
				is_err = 1;
				break;
			}
		}
		if (is_err != 1)
			if (flag != 3 || argv[cur_argc][i - 1] == '.')
				is_err = 2;
	}
	if (is_err == 2)
	{
		cout << "IP地址错误" << endl;
		return 0;
	}
	if (is_err == 1)
	{
		cout << "参数[" << argv[cur_argc] << "]错误" << endl;
		return 0;
	}
	args_analyse_print(args);
	cout << "ip地址           " << argv[cur_argc] << endl;
	return 0;
}

