
//ʵ�� ö�ٷ������	
/***************************************************************
* example:
* -----main.cpp
*
* #include "enum_map.h"
* //����ö�٣�//enum ֵ�ı��ʽĿǰֻ֧��  + �� - �� * �� / �� << �� >> �� ������ �ַ���ö��ֵ    ������
* enum_define( _myEnum,		//ö������
*
*				_A_1dddd3		=	1000,	//ö��ֵ
*				_A_1dddd32		=	(_A_1dddd3<< 5) + 2,
*				_A_1dddd34		=	11000,
*				_A_1dddd35		=	1200,
*				__sdfsdf_AD_4	=	0xAdef12,
*				_Asdfsdfdd		=	01232
*			  )
*
*	//std::string toStringByEnumValue(enum_name,enum_value_int);
*	//�˺�������ö��string����
*	//@enum_name		����ö������ ���������涨�����Ͷ���
*	//@enum_value_int	����ö���ֶ�ʵ��intֵ
*	//return			stringö���ֶ�����
*
* //����
* int main()
* {
*	printf("%s\n", toStringByEnumValue(_myEnum, 1300l).c_str());
*	printf("%s\n", toStringByEnumValue(_myEnum, 1200).c_str());
*	printf("%s\n", toStringByEnumValue(_myEnum+, 1000).c_str());
*	return 0;
* }
*
**************************************************************************/

#ifndef _enum_map_h_h
#define _enum_map_h_h
#include <stdarg.h>
#include <string>
#include <map>
#include <vector>

typedef std::map<int, std::string> enum_map;
typedef std::map<std::string, enum_map>	enum_map_list;

extern enum_map_list all_enum_map_list;

std::string TostringByValue(const std::string enum_name, const  std::string cs_enum_info, const  int enum_value);

#define enum_define(enum_name,...)\
enum enum_name{\
	__VA_ARGS__\
};\
	const char	enum_str_##enum_name[] = #__VA_ARGS__;

#define toString(value_)	#value_

#define toStringByEnumValue(enum_name,enum_value) TostringByValue(#enum_name,enum_str_##enum_name,enum_value)


#endif