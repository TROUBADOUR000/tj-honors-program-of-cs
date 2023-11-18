/* 2153592 ���ݷ� ��01 */
#pragma once

#define ENABLE_LIB_TCFG_TOOLS   0

#if ENABLE_LIB_TCFG_TOOLS

#include "../include/lib_tcfg_tools.h"
typedef tcft cft;	//��tcft����Ϊcft

#else

/* Ŀǰ�ճ�lib_tcfg_tools.h�������涨Ҫ�����仯 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

enum class ITEM_TYPE {
	Int,		//Ϊ�����int��ͻ����������ĸ��д��ʽ
	Double,
	String,		//string����ʽ(�� char * / char [])
	Character,	//char ��ʽ
	Line,		//cft_gai�еĴ��ַ�����ʽ
	Null
};

#define OPEN_OPT_RDONLY		0	//��ֻ����ʽ�򿪣��򲻿��򷵻�ʧ�ܣ�
#define OPEN_OPT_RDWR		1	//�Զ�д��ʽ�򿪣��򲻿��ļ��򴴽���

/* ---------------------------------------------------------------
	 �������������Ҫ�Ķ���
	 constȫ�ֱ���/staticȫ�ֱ���(����)/define/class/struct/enum/enum class/union��
   ---------------------------------------------------------------- */
   /* ���������ļ�����/����ӵ�λ��

   /* �������λ�ã�
	   �������������ļ����������һ�С��������һ�����С����������һ��Ŀ�ʼ��
	   �������ļ�    �������ļ����ʼ�������ļ��Ľ�������һ�����к� */
enum class InsertPosition {
	//�������ļ�������λ��
	First_line_after_group = 0,	//�������һ��
	First_blank_line_after_group,	//�������һ������
	Before_next_group,			//���������һ��Ŀ�ʼ��
	//�������ļ�������λ��
	Begin_of_file,				//�����ļ����ʼ�������ã�
	First_blank_line,				//��һ������λ�ã������ã�
	End_of_file,					//�����ļ�����󣨼����ã�
};

/* �����Ƿ����ע����䣺
	 on ����#��;��ʼ��������βΪע����䣬���Ե�
	 off��#��;������Ч�ַ����� */
enum class CommentSwitch {
	on = 0,		//�������һ��
	off
};

/* ---------------------------------------------------------------
	 class CFT�Ķ��壬���в��ֲ�Ҫ��������protected���ּ���������Ҫ�Ķ��壨�������ݳ�Ա�ͳ�Ա������
   ---------------------------------------------------------------- */
class cft {
protected:
	string filename;
	fstream fp;
	InsertPosition inspos;
	CommentSwitch is_ignore_comment;

	ostringstream msg;	//���ڴ���ڲ���Ϣ

	int file_resize(int newsize); //��ʵ�֣���Ҫ��

	bool get_group_name(string line, string& name);
	bool is_group_existed(const char* group_name);
	bool is_simple_file();
	string get_last_name();
	int num_of_item(const char* group_name, const char* item_name);
	/* ---------------------------------------------------------------
		 �������������Ҫ�Ķ��壨�ڲ��ã������⣬��֤�����СΪ768��
	   ---------------------------------------------------------------- */
	char pad[384];

public:
	cft();		//�޲ι��죬��filenameΪ""����
	cft(const char* cfgname, int opt = OPEN_OPT_RDONLY);	//���ι��죬����ͬopen
	cft(const string& cfgname, int opt = OPEN_OPT_RDONLY);	//���ι��죬����ͬopen
	~cft();		//����

	void open(const char* cfgname, int opt = OPEN_OPT_RDONLY);
	void open(const string& cfgname, int opt = OPEN_OPT_RDONLY);
	bool is_open();	//����򿪳ɹ����򷵻�true�����򷵻�false
	void close();

	void print() const;	//��ӡ�ڲ�msg��Ϣ(���ڵ���)

	/* �����Ƿ����ע����䣨Ĭ�ϣ�yes�� */
	void set_comment_switch(CommentSwitch flag);
	CommentSwitch get_comment_switch() const;

	/* ����item_add�Ĳ���λ�ã�Ĭ�ϣ�First_line_after_group�� */
	void set_insert_position(InsertPosition pos);
	InsertPosition get_insert_position() const;

	/* ��������ɾ�� */
	int group_add(const char* group_name);
	int group_add(const string& group_name);
	int group_del(const char* group_name);
	int group_del(const string& group_name);

	/* �����ӣ����������� */
	int item_add(const char* group_name, const char* item_name, const int item_value);
	int item_add(const char* group_name, const char* item_name, const double item_value);
	int item_add(const char* group_name, const char* item_name, const char* item_value);
	int item_add(const char* group_name, const char* item_name, const string& item_value);
	int item_add(const char* group_name, const char* item_name, const char item_value);
	int item_add(const char* group_name, const char* item_name);	//NULL

	/* ��ɾ�� */
	int item_del(const char* group_name, const char* item_name);
	int item_del(const char* group_name, const string& item_name);

	/* ����£����������� */
	int item_update(const char* group_name, const char* item_name, const int item_value);
	int item_update(const char* group_name, const char* item_name, const double item_value);
	int item_update(const char* group_name, const char* item_name, const char* item_value);
	int item_update(const char* group_name, const char* item_name, const string& item_value);
	int item_update(const char* group_name, const char* item_name, const char item_value);
	int item_update(const char* group_name, const char* item_name);	//NULL

	/* ȡĳ���ֵ������������ */
	int item_get_value(const char* group_name, const char* item_name, int& item_value);
	int item_get_value(const char* group_name, const char* item_name, double& item_value);
	int item_get_value(const char* group_name, const char* item_name, char* item_value, bool space_enable = false); //���һ������Ϊtrue��ʾ������ֵ�����ո�
	int item_get_value(const char* group_name, const char* item_name, string& item_value, bool space_enable = false);
	int item_get_value(const char* group_name, const char* item_name, char& item_value);
	int item_get_value(const char* group_name, const char* item_name);

	/* ---------------------------------------------------------------
		 ���������������Ҫ�Ķ��壨��������ֵ�ֻ���������
	   ---------------------------------------------------------------- */
};

#endif
