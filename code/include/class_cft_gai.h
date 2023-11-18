/* 2153592 ���ݷ� ��01 */
#pragma once

#include "../include/class_cft.h"

#if ENABLE_LIB_TCFG_TOOLS

#include "../include/lib_tcfg_tools_get_all_item.h"
typedef tcft_gai	cft_gai;		//��tcft_gai����Ϊcft_gai

#else

/* Ŀǰ�ճ�lib_tcfg_tools_get_all_items.h�������涨Ҫ�����仯 */
/* gai = get_all_item ��get_all_item_from_one_group */
class cft_gai : public  cft {
protected:
	char pad[256];

public:
	int item_add(const char* group_name, const char* item_value);	//��ctf��ͬ��NULL
	int item_add(const char* group_name, const string& item_value);	//��ctf��ͬ��NULL

	int item_update() const; //ͨ�����ַ�����ֹʹ��ctf�������update������ǿ�Ƽ�ctf::�������Ʒ�Χ�ڣ�

	int item_del(const char* group_name, const char* item_value);
	int item_del(const char* group_name, const string& item_value);

	string* item_get_all(const char* group_name);
};

#endif
