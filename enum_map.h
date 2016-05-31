
//实现 枚举反射机制	
/***************************************************************
* example:
* -----main.cpp
*
* #include "enum_map.h"
* //定义枚举：//enum 值的表达式目前只支持  + ， - ， * ， / ， << ， >> ， （）， 字符型枚举值    操作符
* enum_define( _myEnum,		//枚举名称
*
*				_A_1dddd3		=	1000,	//枚举值
*				_A_1dddd32		=	(_A_1dddd3<< 5) + 2,
*				_A_1dddd34		=	11000,
*				_A_1dddd35		=	1200,
*				__sdfsdf_AD_4	=	0xAdef12,
*				_Asdfsdfdd		=	01232
*			  )
*
*	//std::string toStringByEnumValue(enum_name,enum_value_int);
*	//此函数返回枚举string类型
*	//@enum_name		输入枚举名称 必须用上面定义类型定义
*	//@enum_value_int	输入枚举字段实际int值
*	//return			string枚举字段名称
*
* //调用
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