/* 2153592 ���ݷ� ��01 */
/* 2152611 ����˳ 2150482 �ƽ��� 2152971 ��˼�� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <io.h>
#include <stdio.h>
#include <conio.h>
using namespace std;

#include "../include/class_cft.h"

union cfg_value {
	int ivalue;
	double dvalue;
	char svalue[256];//����ַ������ȳ���255����ֻȡǰ255���ַ�
	char cvalue;
	void* value;
};

struct cfg_item {
	const char* cfg_item_name;
	enum ITEM_TYPE cfg_item_type;
	union cfg_value cfg_item_value;
};

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ÿ�β���ǰ����Ҫɾ���ļ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static void ask_for_remove_cfgfile(const char* const cfgname, const char ch_yes = 'Y', const char ch_no = 'N')
{
	char sel;

	/*  �ļ���������ֱ�ӷ��� */
	if (_access(cfgname, 0x00) < 0)
		return;

	cout << "��ѡ���Ƿ�ɾ���ļ�(" << cfgname << ")[" << ch_yes << "=ɾ��/" << ch_no << "=����] : " << endl;

	while (1) {
		sel = _getch();
		if (sel == ch_yes || sel == ch_no)
			break;
	}

	/* ���ܻ���Ϊ���ڲ鿴�ļ�������ɾ�����ɹ���Ҫ����ɾ */
	while (sel == ch_yes) {
		if (remove(cfgname) == 0)
			break; //ɾ���ɹ����˳�ѭ��

		cout << "�ļ�(" << cfgname << ")ɾ��ʧ�ܣ��������Ϊ���ڲ鿴���ļ�����رպ󰴻س��ٴγ���ɾ��" << endl;
		while (_getch() != '\r')
			;
	}

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾ��ʾ��Ϣ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static void to_be_continued(const char* const prompt = NULL, const char ch = '\r')
{
	char tmp[64];

	/*�������ʽ��2/3�����������ͣ�����һ����һ���ַ�(ch=='\r' ? "�س�" : ch)�����Ҫ���⴦��һ�� */
	if (ch == '\r')
		sprintf(tmp, "�س�");
	else
		sprintf(tmp, "%c[%d]", ch, ch);

	if (prompt)
		cout << prompt << "����" << tmp << "������...";
	else
		cout << "��" << tmp << "������...";

	/* ���Գ��س�������������루ע�⣺_getch�Ļس���\r����getchar��\n��*/
	while (_getch() != ch)
		;
	cout << endl;

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���Ϊcfg_item�к��й����壬������ֻ�ܳ�ʼ����һ�����ú�����ʼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static void set_item(struct cfg_item* item)
{
	item[0].cfg_item_value.ivalue = 123;
	item[1].cfg_item_value.dvalue = 123.45;
	strcpy(item[2].cfg_item_value.svalue, "Hello<char *>");
	item[3].cfg_item_value.cvalue = 'Y';
	//	item[4].cfg_item_value.ivalue = 0;
	item[5].cfg_item_value.ivalue = 99999;
	strcpy(item[6].cfg_item_value.svalue, "Hello<String>");
	//	item[7].cfg_item_value.ivalue = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������ļ�����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int test_readconfig_cpp(const char* cfgname, struct cfg_item* item, InsertPosition pos)
{
	cft	fcfg;
	int ret;

	//�������ļ�������λ��
	if (pos< InsertPosition::Begin_of_file || pos > InsertPosition::End_of_file) {
		cout << "ָ���Ĳ���λ�ò���ȷ." << endl;
		return -1;
	}

	/* ��1����ԣ���������� */
	if (1) {
		int j;

		ask_for_remove_cfgfile(cfgname);

		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		for (j = 0; item[j].cfg_item_name; j++) {
			/* ˵����item_add�ĵ�3��������void*����˲���ʲô���ͣ�������union��һ����Ա�ĵ�ַ����������Ա��ַ */
			switch (item[j].cfg_item_type) {
			case ITEM_TYPE::Int:
				ret = fcfg.item_add(NULL, item[j].cfg_item_name, item[j].cfg_item_value.ivalue);
				break;
			case ITEM_TYPE::Double:
				ret = fcfg.item_add(NULL, item[j].cfg_item_name, item[j].cfg_item_value.dvalue);
				break;
			case ITEM_TYPE::String:
				if (j == 2)
					ret = fcfg.item_add(NULL, item[j].cfg_item_name, item[j].cfg_item_value.svalue);
				else
					ret = fcfg.item_add(NULL, item[j].cfg_item_name, string(item[j].cfg_item_value.svalue)); //ͨ����ʽ����string��ģ��string
				break;
			case ITEM_TYPE::Character:
				ret = fcfg.item_add(NULL, item[j].cfg_item_name, item[j].cfg_item_value.cvalue);
				break;
			case ITEM_TYPE::Null:
				ret = fcfg.item_add(NULL, item[j].cfg_item_name);
				break;
			default:
				break;
			}
			cout << "����" << item[j].cfg_item_name << (ret > 0 ? "�ɹ�." : "ʧ��.") << endl;
		}

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��1�������ɣ���������ļ�(" << cfgname << ")���۲��Ƿ���6��������" << endl;
		to_be_continued();
	}

	/* ��2����ԣ�ɾ���� */
	if (1) {
		cout << "��������ļ�(" << cfgname << ")����������B�������ɷݣ��Һ����ֵ���Ͳ�ͬ" << endl;
		to_be_continued();

		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		cout << "ɾ��<������B>��" << fcfg.item_del(NULL, "������B") << "��" << endl;
		cout << "ɾ��<������G>��" << fcfg.item_del(NULL, "������G") << "��" << endl;

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��2�������ɣ���������ļ�(" << cfgname << ")���۲�<������B>/<������G>�Ƿ��ѱ�ɾ��" << endl;
		to_be_continued();
	}

	/* ��3����ԣ��޸��� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		int i = 54321;
		double d = 1023.4567;
		char c = 'N';
		string s = "helloworld <string>";

		/* �Ѵ�������� */
		fcfg.item_update(NULL, "������D", "helloworld");
		fcfg.item_update(NULL, "������E", d);
		fcfg.item_update(NULL, "������F");
		fcfg.item_update(NULL, "������C", c);
		fcfg.item_update(NULL, "������A", s);

		/* ����������� */
		fcfg.item_update(NULL, "������u");
		fcfg.item_update(NULL, "������v", c);
		fcfg.item_update(NULL, "������w");
		fcfg.item_update(NULL, "������x", i);
		fcfg.item_update(NULL, "������y", "helloworld");
		fcfg.item_update(NULL, "������z", d);
		fcfg.item_update(NULL, "������q", s);
		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��3�������ɣ���������ļ�(" << cfgname << ")���۲����������Ƿ����޸�" << endl;
		to_be_continued();
	}

	/* ��4����ԣ��� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDONLY);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		int i;
		double d;
		char c, s[128];
		string str;

		/* �� */
		if (fcfg.item_get_value(NULL, "������x", i) > 0)
			cout << "������x=" << i << endl;
		if (fcfg.item_get_value(NULL, "������v", c) > 0)
			cout << "������v=" << c << endl;
		if (fcfg.item_get_value(NULL, "������y", s) > 0)
			cout << "������y=" << s << endl;
		if (fcfg.item_get_value(NULL, "������q", str) > 0)//ֵ��������ո�
			cout << "������q=" << str << endl;
		if (fcfg.item_get_value(NULL, "������q", str, true) > 0) //ֵ������ո�
			cout << "������q=" << str << endl;
		if (fcfg.item_get_value(NULL, "������w") > 0)
			cout << "������w=<NULL>" << endl;
		if (fcfg.item_get_value(NULL, "������E", d) > 0)
			cout << "������E=" << d << endl;
		if (fcfg.item_get_value(NULL, "������100") > 0)
			cout << "������100=<NULL>" << endl;
		else
			cout << "������100 ������" << endl;

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��4�������ɣ���������ļ�(" << cfgname << ")���۲��������ö�ȡ�Ƿ���ȷ" << endl;
		to_be_continued();
	}

	return 0;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int test_readconfig_cpp(const char* cfgname, const char** group_name, struct cfg_item* item, InsertPosition pos)
{
	cft	fcfg;
	int ret, i;
	char tmp[256];

	//�������ļ�������λ��
	if (pos< InsertPosition::First_line_after_group || pos > InsertPosition::Before_next_group) {
		cout << "ָ���Ĳ���λ�ò���ȷ." << endl;
		return -1;
	}

	/* ��1����ԣ���������� */
	if (1) {
		ask_for_remove_cfgfile(cfgname); //ɾ���ļ�

		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		for (i = 0; group_name[i]; i++) {
			if ((ret = fcfg.group_add(group_name[i])) <= 0)
				cout << "����[" << group_name[i] << "]ʧ��." << endl;
			else
				cout << "����[" << group_name[i] << "]�ɹ�." << endl;
		}
		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��1�������ɣ���������ļ�(" << cfgname << ")���۲��Ƿ����8������";
		to_be_continued();
	}

	/* ��2����ԣ�ÿ����������� */
	if (1) {
		cout << "��������ļ�(" << cfgname << ")����[������2/7]��Ϊ[  ������2  ]/[ ������7]" << endl;
		to_be_continued();

		int j;

		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		for (i = 0; group_name[i]; i++) {
			for (j = 0; item[j].cfg_item_name; j++) {
				sprintf(tmp, "%s-%s", group_name[i], item[j].cfg_item_name);
				/* ˵����item_add�ĵ�3��������void*����˲���ʲô���ͣ�������union��һ����Ա�ĵ�ַ����������Ա��ַ */
				switch (item[j].cfg_item_type) {
				case ITEM_TYPE::Int:
					ret = fcfg.item_add(group_name[i], tmp, item[j].cfg_item_value.ivalue);
					break;
				case ITEM_TYPE::Double:
					ret = fcfg.item_add(group_name[i], tmp, item[j].cfg_item_value.dvalue);
					break;
				case ITEM_TYPE::String:
					if (j == 2)
						ret = fcfg.item_add(group_name[i], tmp, item[j].cfg_item_value.svalue);
					else
						ret = fcfg.item_add(group_name[i], tmp, string(item[j].cfg_item_value.svalue)); //ͨ����ʽ����string��ģ��string
					break;
				case ITEM_TYPE::Character:
					ret = fcfg.item_add(group_name[i], tmp, item[j].cfg_item_value.cvalue);
					break;
				case ITEM_TYPE::Null:
					ret = fcfg.item_add(group_name[i], tmp);
					break;
				default:
					break;
				}
				cout << "����[" << group_name[i] << "]/" << tmp << (ret > 0 ? "�ɹ�." : "ʧ��.") << endl;
			}
		}
		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��2�������ɣ���������ļ�(" << cfgname << ")���۲��Ƿ�8�������6��������" << endl;
		to_be_continued();
	}

	/* ��3����ԣ�ɾ���鼰�� */
	if (1) {
		cout << "��������ļ�(" << cfgname << ")����[������3]�������ɷݣ�������2-������B�������ɷݣ��Һ����ֵ���Ͳ�ͬ" << endl;
		to_be_continued();

		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		cout << "ɾ��[������3]��" << fcfg.group_del("������3") << "��" << endl;
		cout << "ɾ��[������5]��" << fcfg.group_del("������5") << "��" << endl;

		cout << "ɾ��<������2-������B>��" << fcfg.item_del("������2", "������2-������B") << "��" << endl;

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��3�������ɣ���������ļ�(" << cfgname << ")���۲�[������3]/[������5]/<������2-������B>�Ƿ��ѱ�ɾ��" << endl;
		to_be_continued();
	}

	/* ��4����ԣ��޸��� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		int i = 54321;
		double d = 1023.4567;
		char c = 'N';
		string s = "helloworld<string>";

		/* �Ѵ�������� */
		fcfg.item_update("[ ������4 ]", "[ ������4 ]-������B", i);
		fcfg.item_update("[������1]", "[������1]-������D", "helloworld");
		fcfg.item_update("������8", "������8-������E", d);
		fcfg.item_update("**������6**", "**������6**-������F");
		fcfg.item_update("������7", "������7-������C", c);
		fcfg.item_update("������2", "������2-������D", s);

		/* ����������� */
		fcfg.item_update("**������6**", "**������6**-������U");
		fcfg.item_update("������7", "������7-������V", c);
		fcfg.item_update("������7", "**������6**-������W");
		fcfg.item_update("[ ������4 ]", "[ ������4 ]-������X", i);
		fcfg.item_update("[������1]", "[������1]-������Y", "helloworld");
		fcfg.item_update("������8", "������8-������Z", d);
		fcfg.item_update("������2", "������2-������Q", s);
		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��4�������ɣ���������ļ�(" << cfgname << ")���۲����������Ƿ����޸�" << endl;
		to_be_continued();
	}

	/* ��5����ԣ��� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDONLY);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		int i;
		double d;
		char c, s[128];
		string str;

		/* �Ѵ�������� */
		if (fcfg.item_get_value("������4", "������4-������B", i) > 0)
			cout << "[[ ������4 ]]/[ ������4 ]-������B=" << i << endl;
		if (fcfg.item_get_value("������1", "������1-������F", i) > 0)
			cout << "[[������1]]/[������1]-������F=" << i << endl;
		if (fcfg.item_get_value("������6", "������6-������D", c) > 0)
			cout << "[**������6**]/**������6**-������D=" << c << endl;
		if (fcfg.item_get_value("������8", "������8-������C", s) > 0)			//Ĭ�϶�����item_value���ܴ��ո�
			cout << "[������8]/������8-������C=" << s << endl;
		if (fcfg.item_get_value("������8", "������8-������C", s, true) > 0)	//����true������ʾ������item_value������ո�
			cout << "[������8]/������8-������C=" << s << endl;
		if (fcfg.item_get_value("������2", "������2-������Q", str) > 0)
			cout << "[������2]/������2-������Q=" << str << endl;
		if (fcfg.item_get_value("������7", "������7-������E") > 0)
			cout << "[������7]/������7-������E=<NULL>" << endl;
		if (fcfg.item_get_value("������6", "������6-������B", d) > 0)
			cout << "[**������6**]/**������6**-������B=" << d << endl;
		if (fcfg.item_get_value("������1", "������100") > 0)
			cout << "[������1]/������100=<NULL>" << endl;
		else
			cout << "[������1]/������100 ������" << endl;

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��5�������ɣ���������ļ�(" << cfgname << ")���۲��������ö�ȡ�Ƿ���ȷ" << endl;
		to_be_continued();
	}

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int main()
{
	/* Ϊ�˱����Լ��ĳ������ṩ��lib���Ի��ã���������class�Ĵ�СΪ 1024�ֽڣ��������� */
	if (sizeof(cft) != 768) {
		cout << "class cft ����768�ֽڣ����ʹ�� lib_tcfg_tools.lib �еĺ�������ܳ���" << endl;
		return -1;
	}

	const char* cfgname = "u2153592_g.cfg";
	const char* group_name[] = {
		"[������1]",
		"������2",
		"������3",
		"[ ������4 ]",
		"������5",
		"**������6**",
		"������7",
		"������8",
		NULL
	};

	struct cfg_item item[] =
	{
		{ "������A",ITEM_TYPE::Int, 0 },		//������union�ĳ�ʼ����ֻ�ܳ�ʼ����һ��Ԫ��
		{ "������B",ITEM_TYPE::Double, 0 },
		{ "������C",ITEM_TYPE::String, 0 },
		{ "������D",ITEM_TYPE::Character, 0 },
		{ "������E",ITEM_TYPE::Null, 0 },
		{ "������F",ITEM_TYPE::Int, 0 },
		{ "������G",ITEM_TYPE::String, 0 },
		{ NULL,		ITEM_TYPE::Null, 0 }
	};
	set_item(item); //��union����ֵ
#if 1
	int loop = 1;
	char sel;
	while (loop) {
		cout << "====================================" << endl;
		cout << "�������ļ����ԣ���ѡ����뷽ʽ" << endl;
		cout << "  1.�����������" << endl;
		cout << "  2.��������ĵ�һ������λ��" << endl;
		cout << "  3.����һ������ǰ" << endl;
		cout << "�������ļ����ԣ���ѡ����뷽ʽ" << endl;
		cout << "  4.���ļ��ʼ����" << endl;
		cout << "  5.�ڵ�һ������λ��" << endl;
		cout << "  6.���ļ�������" << endl;
		cout << "  0.�˳�" << endl;
		cout << "====================================" << endl;
		cout << "��ѡ��[0-3] : ";
		sel = _getch();
		putchar(sel);
		putchar('\n');

		switch (sel) {
		case '1':
			test_readconfig_cpp(cfgname, group_name, item, InsertPosition::First_line_after_group);
			to_be_continued("\n\n\n��C����", 'C');
			break;
		case '2':
			test_readconfig_cpp(cfgname, group_name, item, InsertPosition::First_blank_line_after_group);
			to_be_continued("\n\n\n��C����", 'C');
			break;
		case '3':
			test_readconfig_cpp(cfgname, group_name, item, InsertPosition::Before_next_group);
			break;
			to_be_continued("\n\n\n��C����", 'C');
		case '4':
			test_readconfig_cpp(cfgname, item, InsertPosition::Begin_of_file);
			to_be_continued("\n\n\n��C����", 'C');
			break;
		case '5':
			test_readconfig_cpp(cfgname, item, InsertPosition::First_blank_line);
			to_be_continued("\n\n\n��C����", 'C');
			break;
		case '6':
			test_readconfig_cpp(cfgname, item, InsertPosition::Begin_of_file);
			break;
			to_be_continued("\n\n\n��C����", 'C');
		case '0':
			loop = 0;
			break;
		default:
			break;
		}	//end of switch
	}
#elif 0
	/* ��Ϊ���ù����˵���������ѡһ����*/
	test_readconfig_cpp(cfgname, group_name, item, InsertPosition::First_line_after_group);
	to_be_continued("\n\n\n��C����", 'C');

	test_readconfig_cpp(cfgname, group_name, item, InsertPosition::First_blank_line_after_group);
	to_be_continued("\n\n\n��C����", 'C');

	test_readconfig_cpp(cfgname, group_name, item, InsertPosition::Before_next_group);
	to_be_continued("\n\n\n��C����", 'C');
#endif

	return 0;

}
