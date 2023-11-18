/* 2153592 ���ݷ� ��01 */
/* 2152611 ����˳ 2150482 �ƽ��� 2152971 ��˼�� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include "../include/class_cft.h"
#include "../include/class_cft_gai.h"
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾ��ʾ��Ϣ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static void to_be_continued(const char* prompt = NULL, const char ch = '\r')
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
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int test_readconfig_cpp(const char* cfgname, const char** group_name, const char** item, InsertPosition pos)
{
	cft_gai	fcfg;
	int ret, i;

	/* ��1����ԣ���������� */
	if (1) {
		ret = remove(cfgname); //ɾ���ļ�

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

		cout << "��1�������ɣ���������ļ�(" << cfgname << ")���۲��Ƿ����6������";
		to_be_continued();
	}

	/* ��2����ԣ�ÿ����������� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		/* ���������У�ÿ��group��Ӧһ��item��һ��item�пɺ�\r\n */
		for (i = 0; group_name[i]; i++) {
			ret = fcfg.item_add(group_name[i], item[i]);
			cout << "����[" << group_name[i] << "]/" << item[i] << (ret > 0 ? "�ɹ�." : "ʧ��.") << endl;
		}
		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��2�������ɣ���������ļ�(" << cfgname << ")���۲��Ƿ�6�������1��������" << endl;
		to_be_continued();
	}

	/* ��3����ԣ�ɾ��������ĳ�����е���
		ע��1����Ϊ���� name = value ����ʽ�����Ҫɾ�������ݱ�����ȫƥ�䣬����ֻ���� name
			2��ͬ�����ɣ���֧��Update��Update������Delete+Addʵ�� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		/* ���� FIRST_LINE_AFTER_GROUP ����ʽ�������϶�Ӧ��Ŀ�Ķ�ȡҪ���ǹ��߼��Ĵ��� */
		fcfg.group_add("5-b18-err-04");
		fcfg.item_add("5-b18-err-04", "���������볤��(12-16)����д��ĸ����(��2)�� Сд��ĸ����(��2)�����ָ���(��2)���������Ÿ���(��2)");
		fcfg.item_add("5-b18-err-04", "17 2 2 2 2");
		fcfg.item_add("5-b18-err-04", "*B{7M4K;llFL:gOCd");
		fcfg.item_add("5-b18-err-04", "Ru(DP!+JlX*h5~2y^");
		fcfg.item_add("5-b18-err-04", "0>{4xqG5@e`rj0hCw");
		fcfg.item_add("5-b18-err-04", "_#K%?Mpa&*L8`2RNQ");
		fcfg.item_add("5-b18-err-04", "dn2FI6+Lyp_GX&6$@");
		fcfg.item_add("5-b18-err-04", ",!luVb<W37U*'51~=");
		fcfg.item_add("5-b18-err-04", "j<:#|Weq9g6A(}YR'");
		fcfg.item_add("5-b18-err-04", "zU2n[l4J|#Pj4qo6]");
		fcfg.item_add("5-b18-err-04", "Tl`f{7|rF4f(S6sP+");
		fcfg.item_add("5-b18-err-04", "(m'����gE^9p;GaYx");

		string s1 = "1234.5678        # �µĲ�������";

		/* ͨ������+ɾ��ʵ�ָ��� */
		fcfg.item_add("3-�ٺ�-01", s1);
		fcfg.item_del("3-�ٺ�-01", item[0]);

		/* ͨ��ɾ��+����ʵ�ָ��� */
		fcfg.item_del("3-�Ǻ�-02", item[1]);
		fcfg.item_add("3-�Ǻ�-02", "0.07");

		/* ��ɾ�� */
		fcfg.item_del("3-����-03", item[2]);

		/* �����루���Գ�����������������ظ������ǹ��ߵĴ� */
		fcfg.item_add("3-b4-04", item[3]);

		/* ������ */
		fcfg.item_add("5-b17-ok-01", "# �����ע����1.23.45.678");

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��3�������ɣ���������ļ�" << cfgname << "���۲����������Ƿ����޸�" << endl;;
		to_be_continued();
	}

	/* ��4����ԣ�ɾ���ղŲ���Ĵ�ע�͵�"1234.5678  # �µĲ�������" */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		string s1 = "        1234.5678        # �µĲ�������";

		/* ɾ���ղŲ����"1234.5678  # �µĲ�������" */
		fcfg.item_del("3-b4-01", s1.substr(0, 20)); //ɾ����Чƥ����"1234.5678"��ǰ�󶼶���˿ո�

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��4�������ɣ���������ļ�" << cfgname << "���۲����������Ƿ����޸�" << endl;;
		to_be_continued();
	}

	/* ��5����ԣ�ɾ��ĳ���� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDWR);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		cout << "ɾ��[3-�Ǻ�-02]��" << fcfg.group_del("3-�Ǻ�-02") << "��" << endl;
		cout << "ɾ��[5-b18-err-04]��" << fcfg.group_del("5-b18-err-04") << "��" << endl;

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������

		cout << "��5�������ɣ���������ļ�(" << cfgname << ")���۲�[3-�Ǻ�-02]/[5-b18-ok-02]�Ƿ��ѱ�ɾ��" << endl;
		to_be_continued();
	}

	/* ��6����ԣ���������ע�ͣ� */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDONLY);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::on);

		/* ѭ������[5-b18-err-04]��ȫ������] */
		string* list = fcfg.item_get_all("5-b18-ok-02");
		int num = 0;

		for (num = 0; list && list[num] != ""; num++)
			cout << num << " : " << list[num] << endl;

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������
		delete[]list; //�ǵ��ͷ�

		cout << "��6�������ɣ���������ļ�(" << cfgname << ")���۲��������ö�ȡ�Ƿ���ȷ" << endl;
		to_be_continued();
	}

	/* ��7����ԣ���(������ע��) */
	if (1) {
		fcfg.open(cfgname, OPEN_OPT_RDONLY);
		if (fcfg.is_open() == false) {
			cout << "�޷��������ļ�(" << cfgname << ")." << endl;
			return -1;
		}
		fcfg.set_insert_position(pos);
		fcfg.set_comment_switch(CommentSwitch::off);

		/* ѭ������[5-b18-err-04]��ȫ������] */
		string* list = fcfg.item_get_all("5-b18-ok-02");
		int num = 0;

		for (num = 0; list && list[num] != ""; num++)
			cout << num << " : " << list[num] << endl;

		fcfg.close(); //�ر��ļ���ģ�Ȿ���������
		delete[]list; //�ǵ��ͷ�

		cout << "��7�������ɣ���������ļ�(" << cfgname << ")���۲��������ö�ȡ�Ƿ���ȷ" << endl;
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
	if (sizeof(cft_gai) != 1024) {
		cout << "class cft ����1024�ֽڣ����ʹ�� lib_tcfg_tools.lib �еĺ�������ܳ���" << endl;
		return -1;
	}

	const char* cfgname = "u2153592_gai.txt";
	const char* group_name[] = {
		"3-�ٺ�-01",
		"3-�Ǻ�-02",
		"3-����-03",
		"3-$$-04",
		"5-b17-ok-01",
		"5-b18-ok-02",
		NULL
	};

	const char* item[] =
	{
		"9999999999.99",
		"0.35           # ���ڲ��ԵĶ���ע������",
		"10001.34       ###",
		"0.07  $$",
		"12 2 2 2 2",
		/* ע�⣬���������У���һ��û�ж��ţ�����������Ϊһ���������ַ���������һ��ʹ�ü��ɣ����������������ˣ� */
		"���������볤��(12-16)����д��ĸ����(��2)�� Сд��ĸ����(��2)�����ָ���(��2)���������Ÿ���(��2)\r\n12 2 2 2 2\r\n"
		"Ynk-Fq89oE-I\r\n6q2ZApV$bD*@\r\nGy$@wnQ4;bP6\r\nPXy%6*e9#o^2\r\n9i@PH%pA1mnk\r\nSu1Tr4!CT*kS\r\nmx*-6p+Sl2gT\r\nj@1r,2VCb?DI\r\n?oAun7V%!97_\r\n31yR,&D4.opV",
		NULL
	};

#if 1
	int loop = 1;
	char sel;
	while (loop) {
		cout << "============================" << endl;
		cout << "��ѡ����뷽ʽ" << endl;
		cout << "  1.�����������" << endl;
		cout << "  2.��������ĵ�һ������λ��" << endl;
		cout << "  3.����һ������ǰ" << endl;
		cout << "  0.�˳�" << endl;
		cout << "============================" << endl;
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
		case '0':
			loop = 0;
			break;
		default:
			break;
		}	//end of switch
	}
#else
	/* ��Ϊ���ù����˵���������ѡһ����*/
	//test_readconfig_cpp(cfgname, group_name, item, InsertPosition::First_line_after_group);
		//to_be_continued("\n\n\n��C����", 'C');

		test_readconfig_cpp(cfgname, group_name, item, InsertPosition::First_blank_line_after_group);
		to_be_continued("\n\n\n��C����", 'C');

		test_readconfig_cpp(cfgname, group_name, item, InsertPosition::Before_next_group);
		to_be_continued("\n\n\n��C����", 'C');
#endif

	return 0;

}
