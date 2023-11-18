/* 2153592 ���ݷ� ��01 */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../include/class_aat.h"
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������
  ���������
  �� �� ֵ��
  ˵    ���������и�ʽҪ����--��ʼ�Ŀɱ������˳�����⣬�����ָ���Ĺ̶�����
***************************************************************************/
int main(int argc, char* argv[])
{
	/* ��Ϊ������ lib_aat_tools.lib���˴��ȼ���С�Ƿ����Ҫ�� */
	if (sizeof(args_analyse_tools) != 200) {
		cout << "class args_analyse_tools ����200�ֽڣ����ʹ�� lib_aat_tools.lib �еĺ�������ܳ���" << endl;
		return -1;
	}

	string hashtype[] = { "md5","sha1","sha224","sha256","sha384","sha512","all", "" };	//��β������""
	int intset[] = { 11, 22, 33, 123, 345, INVALID_INT_VALUE_OF_SET }; 				//��β������INVALID_INT_VALUE_OF_SET

	args_analyse_tools args[] = {
		args_analyse_tools("--help",      ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--bool",      ST_EXTARGS_TYPE::boolean, 0, true),
		args_analyse_tools("--intdef",    ST_EXTARGS_TYPE::int_with_default,        1, 12345, 0, 65535),
		args_analyse_tools("--interr",    ST_EXTARGS_TYPE::int_with_error,          1, 12345, 0, 65535),	//����12345��Ϊ�κ�ֵ��������
		args_analyse_tools("--intsetdef", ST_EXTARGS_TYPE::int_with_set_default,    1, 2, intset),		//����2����defaultΪset�еĵ�[2]��ֵ����33�������ֵ����Χ��ȱʡ0
		args_analyse_tools("--intseterr", ST_EXTARGS_TYPE::int_with_set_error,      1, 2, intset),		//����2�����壬����
		args_analyse_tools("--str1",      ST_EXTARGS_TYPE::str,                  1, string("")),		//�����string()���������䵽args_analyse(const char *arg_name, enum ST_EXTARGS_TYPE type, int ext_num, bool def)
		args_analyse_tools("--str2",      ST_EXTARGS_TYPE::str,                  1, string("Hello")),	//�����string()���������䵽args_analyse(const char *arg_name, enum ST_EXTARGS_TYPE type, int ext_num, bool def)
		args_analyse_tools("--strsetdef", ST_EXTARGS_TYPE::str_with_set_default, 1, 3, hashtype),		//����3����defaultΪset�еĵ�[3]��ֵ����sha256�������ֵ����Χ��ȱʡ0
		args_analyse_tools("--strseterr", ST_EXTARGS_TYPE::str_with_set_error,   1, 3, hashtype),		//����3�����壬����
		args_analyse_tools("--ipdef",     ST_EXTARGS_TYPE::ipaddr_with_default,     1, string("192.168.80.230")),
		args_analyse_tools("--iperr",     ST_EXTARGS_TYPE::ipaddr_with_error,       1, string("")),
		args_analyse_tools()  //���һ�������ڽ���
	};

	int need_fixed_args = 0;	//���Ը�Ϊ1
	int cur_argc;

	/* ���һ��������1-��ʾ��ѡ������⣬����Ҫ��������
				  0-��ʾ��ѡ������⣬����Ҫ�������� */
	if ((cur_argc = args_analyse_process(argc, argv, args, need_fixed_args)) < 0)
	{
		//������Ϣ�ں������Ѵ�ӡ
		return -1;
	}

	args_analyse_print(args);

	/* ����������ɺ�Ĵ����� */
	if (need_fixed_args) {
		if (cur_argc == argc) {  //˵������û�и���Ҫ�Ĺ̶�����
			cout << "ָ����Ҫ�̶���������δ����" << endl;
			return -1;
		}

		/* �˴�Ӧ�ô���Ҫ��Ĺ̶������������м򵥴�ӡ���� */
		cout << "����" << argc - cur_argc << "���̶�����������ĳ��������Ӧ�Ķ����ɾ�������������������" << endl;
		for (int i = cur_argc; i < argc; i++)
			cout << argv[i] << endl;
	}

	cout << endl << "������۲��ӡ����У���Ӧ���exist�Ƿ�Ϊ1��ֵ�Ƿ�ΪԤ��" << endl << endl;

	cout << "�鿴default��error����ķ�����" << endl;
	cout << "  ��������ָ����Χ�ڵ�ֵ���ῴ�����" << endl;
	cout << "  ����--intdef 100000" << endl;
	cout << "      --interr 100000" << endl;

	return 0;
}

#if 0
// ��������

. / test_optargs --help

. / test_optargs--bool

. / test_optargs --intdef 123
. / test_optargs --intdef 123 --intdef 456
. / test_optargs --intdef - 12
. / test_optargs --intdef 654321

. / test_optargs --interr 123
. / test_optargs --interr 123 --interr 456	//����
. / test_optargs --interr - 12
. / test_optargs --interr 654321

. / test_optargs --intsetdef 11
. / test_optargs --intsetdef 12
. / test_optargs --intsetdef 11 --intsetdef 22

. / test_optargs --intseterr 11
. / test_optargs --intseterr 12
. / test_optargs --intseterr 11 --intseterr 22

. / test_optargs --str1 hello
. / test_optargs --str1 hello --str1 horse	//����

. / test_optargs --str2 hello
. / test_optargs --str2 hello --str2 horse	//����

. / test_optargs --strsetdef md5
. / test_optargs --strsetdef md4
. / test_optargs --strsetdef md5 --strsetdef sha1	//����

. / test_optargs --strseterr md5
. / test_optargs --strseterr md4
. / test_optargs --strseterr md5 --strseterr sha1	//����

. / test_optargs --ipdef 1.1.1.1
. / test_optargs --iperr 1.1.1.1234
. / test_optargs --ipdef 1.1.1.1 --ipdef 2.2.2.2	//����

//������������������ϣ�
#endif
