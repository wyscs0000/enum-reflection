#include "enum_map.h"
#include <stdio.h>
#include <list>

enum_map_list all_enum_map_list;

typedef std::map<std::string, int> enum_stringKey;
typedef std::map<std::string, enum_stringKey>	enum_map_stringKey_list;
enum_map_stringKey_list all_enum_stringKey_list;

//运算符类型
enum exp_type
{
	add_,		//加
	sub_,		//减
	mul_,		//乘
	exc_,		//除
	lmove_,		//左移
	rmove_,		//右移
	lbracket_,	//左括号
	rbracket_,	//右括号
	moreThan_,	//大于
	lessThan_,	//小于
	value_type_count,
};



//运算值
struct str_valueType
{
	int  nValue;
	bool bIsExpOpt;
	bool isExpOpt(){ return bIsExpOpt; }
	void setValue(int value_){ nValue = value_; bIsExpOpt = false; }
	void setOpt(int value_) { nValue = value_; bIsExpOpt = true; }
	int		GetValue(){ return nValue; }
};
typedef std::list<str_valueType> exp_value_list;	//表达式运算值 list

//获取运算类型
exp_type GetExp_type(char x)
{
	if (x == '+')
		return add_;
	if (x == '-')
		return sub_;
	if (x == '*')
		return mul_;
	if (x == '/')
		return exc_;
	if (x == '<')
		return lmove_;
	if (x == '>')
		return rmove_;
	if (x == '(')
		return lbracket_;
	if (x == ')')
		return rbracket_;
	return value_type_count;
}
int Get_exp_value(enum_stringKey& stringKeyList, std::string& str)
{
	if (str.empty())
		return 0;
	int nEnumValue = 0;
	if (str.find("0x") != std::string::npos)	//16进制
		nEnumValue = strtol(str.c_str(), NULL, 16);
	else if (str[0] == '0')//8进制
		nEnumValue = strtol(str.c_str(), NULL, 8);
	else
	{
		//检测是否是枚举值
		bool bEnum = false;
		for (int i = 0; i < str.length(); i++)
		{
			if ('0' > str[i] || str[i] > '9')
			{
				bEnum = true;
				break;
			}
		}
		if (bEnum)	//枚举值
		{
			//优先从所在枚举的 枚举值列表里获取
			enum_stringKey::iterator it;
			it = stringKeyList.find(str);
			if (it != stringKeyList.end())
				nEnumValue = it->second;
			else
			{
				enum_map_stringKey_list::iterator itor = all_enum_stringKey_list.begin();
				for (; itor != all_enum_stringKey_list.end(); itor++)
				{
					it = itor->second.find(str);
					if (it != itor->second.end())
					{
						nEnumValue = it->second;
						break;
					}
				}
			}
		}
		else
			nEnumValue = strtol(str.c_str(), NULL, 10);//10进制
	}
	return nEnumValue;
}
//运算
int Exp_(int nLeft, int nRight, exp_type opt)
{
	switch (opt)
	{
		case add_:
		return nLeft + nRight;
		case sub_:
		return nLeft - nRight;
		case mul_:
		return nLeft * nRight;
		case exc_:
		return nRight ? (nLeft / nRight) : 0;
		case lmove_:
		return nLeft << nRight;
		case rmove_:
		return nLeft >> nRight;
		default:
		return 0;
	}
}

//解析运算表达式的值 和运算符号
bool parse_exp_value(std::string& str, exp_value_list& valueList, enum_stringKey& stringKeyList)
{
	if (str.empty())
		return false;
	std::string tmpstr;
	exp_type _ret_type;
	for (int i = 0; i < str.length();)
	{
		_ret_type = GetExp_type(str[i]);
		if (value_type_count == _ret_type)
		{
			tmpstr += str[i++];
		}
		else
		{
			if (!tmpstr.empty())
			{
				str_valueType tmp_value_;
				tmp_value_.setValue(Get_exp_value(stringKeyList, tmpstr));
				valueList.push_back(tmp_value_);
			}
			str_valueType tmp_opt_;
			tmp_opt_.setOpt(_ret_type);
			valueList.push_back(tmp_opt_);
			tmpstr.clear();
			if (_ret_type == lmove_ || _ret_type == rmove_)
			{
				i += 2;
			}
			else
				i++;
		}
	}
	if (!tmpstr.empty())
	{
		str_valueType tmp_value_;
		tmp_value_.setValue(Get_exp_value(stringKeyList, tmpstr));
		valueList.push_back(tmp_value_);
	}
	return true;
}
//去空格
std::string earse_spel(const std::string& str)
{
	if (str.empty())
		return std::string("");
	std::string tmpStr;
	for (int i = 0; i < str.length(); i++)
		if (str[i] != ' ')
			tmpStr += str[i];
	return tmpStr;
}
//enum 值的表达式目前只支持 + - * / <<  >>  （）操作符
//解析表达式
int parse_expression(exp_value_list& valueList)
{
	if (valueList.size() == 0)
		return 0;
	if (valueList.size() == 1)
		return valueList.begin()->GetValue();

	//先解析最高优先级 括号 
	exp_value_list::iterator itor = valueList.begin();
	exp_value_list::iterator breacket_begin = valueList.end();
	exp_value_list::iterator breacket_end = valueList.end();
	int nIndex = 0;
	for (; itor != valueList.end();)
	{
		if (itor->isExpOpt() && itor->GetValue() == lbracket_)
		{
			if (nIndex == 0)
				breacket_begin = itor;
			nIndex += 1;
		}
		if (itor->isExpOpt() && itor->GetValue() == rbracket_)
		{
			nIndex -= 1;
			if (nIndex == 0)
			{
				breacket_end = itor;
				if (breacket_begin == valueList.end() || breacket_end == valueList.end())
					return 0;
				//一对括号内的运算表达式 递归解析
				exp_value_list _tmp_list;
				breacket_begin++;
				_tmp_list.assign(breacket_begin, breacket_end);
				int nValue_ = parse_expression(_tmp_list);

				//删除括号内的表达式
				breacket_begin--;
				breacket_end++;
				itor = valueList.erase(breacket_begin, breacket_end);

				//插入运算后的值到原先指定位置
				str_valueType tm_value;
				tm_value.setValue(nValue_);
				if (itor == valueList.begin())
					itor = valueList.insert(valueList.begin(), tm_value);
				else if (itor == valueList.end())
				{
					valueList.push_back(tm_value);
					itor = valueList.end();
				}
				else
				{
					itor = valueList.insert(itor, tm_value);
				}

			}
		}
		itor++;
	}
	if (valueList.size() == 1)
		return valueList.begin()->GetValue();


	//运算表达式计算 ---前期先用，后期算法改进
	itor = valueList.begin();
	exp_value_list::iterator nLeftOptValue = itor++;
	int nOptType = value_type_count;
	exp_value_list::iterator nRightOptValue;

	while (itor != valueList.end())
	{
		int nRightOptValue = 0;

		if (itor->isExpOpt() && (itor->GetValue() == mul_ || itor->GetValue() == exc_))
		{
			nOptType = itor->GetValue();
			itor++;
			nRightOptValue = itor->GetValue();
			int nRealV = Exp_(nLeftOptValue->GetValue(), nRightOptValue, (exp_type)nOptType);
			itor++;
			itor = valueList.erase(nLeftOptValue, itor);
			str_valueType tm_value;
			tm_value.setValue(nRealV);
			if (itor == valueList.begin())
				itor = valueList.insert(valueList.begin(), tm_value);
			else if (itor == valueList.end())
			{
				valueList.push_back(tm_value);
				itor = valueList.end();
			}
			else
			{
				itor = valueList.insert(itor, tm_value);
			}

		}
		else
			nLeftOptValue = itor++;
	}
	if (valueList.size() == 1)
		return valueList.begin()->GetValue();

	itor = valueList.begin();
	nLeftOptValue = itor++;
	nOptType = value_type_count;
	while (itor != valueList.end())
	{
		int nRightOptValue = 0;

		if (itor->isExpOpt() && (itor->GetValue() == add_ || itor->GetValue() == sub_))
		{
			nOptType = itor->GetValue();
			itor++;
			nRightOptValue = itor->GetValue();
			int nRealV = Exp_(nLeftOptValue->GetValue(), nRightOptValue, (exp_type)nOptType);
			itor++;
			itor = valueList.erase(nLeftOptValue, itor);
			str_valueType tm_value;
			tm_value.setValue(nRealV);
			if (itor == valueList.begin())
				itor = valueList.insert(valueList.begin(), tm_value);
			else if (itor == valueList.end())
			{
				valueList.push_back(tm_value);
				itor = valueList.end();
			}
			else
			{
				itor = valueList.insert(itor, tm_value);
			}

		}
		else
			nLeftOptValue = itor++;
	}
	if (valueList.size() == 1)
		return valueList.begin()->GetValue();

	itor = valueList.begin();
	nLeftOptValue = itor++;
	nOptType = value_type_count;
	int nRealV = 0;
	while (itor != valueList.end())
	{
		int nRightOptValue = 0;

		if (itor->isExpOpt() && (itor->GetValue() == lmove_ || itor->GetValue() == rmove_))
		{
			nOptType = itor->GetValue();
			itor++;
			nRightOptValue = itor->GetValue();
			nRealV += Exp_(nLeftOptValue->GetValue(), nRightOptValue, (exp_type)nOptType);
		}
		nLeftOptValue = itor++;
	}
	return nRealV;
}

bool parse_enum_str(const std::string enum_name, const std::string& enum_str, enum_map& enumKeyValueList)
{
	std::vector<std::string> enum_value_list;
	//解析枚举逗号分隔符
	size_t npos = enum_str.find(",");
	size_t nlastpos = 0;
	while (npos != std::string::npos)
	{
		enum_value_list.push_back(enum_str.substr(nlastpos, npos - nlastpos));
		nlastpos = npos + 1;
		npos = enum_str.find(",", static_cast<unsigned int>(nlastpos));
	}
	if (nlastpos != enum_str.length())
	{
		enum_value_list.push_back(enum_str.substr(nlastpos, enum_str.length() - nlastpos));
	}
	if (enum_value_list.size() == 0)
		return false;

	//解析单个枚举值对应的名称与值
	enum_stringKey tmp_stringKey;
	enumKeyValueList.clear();
	int nDefaultValue = 0;
	for (std::vector<std::string>::iterator itor = enum_value_list.begin(); itor != enum_value_list.end(); itor++)
	{
		std::string str_enum_field = earse_spel(*itor);	//去空格
		long			nEnumValue;
		std::string str_enum_field_name;

		//解析出枚举值对应的名称与值
		int nPos = str_enum_field.find("=");
		if (nPos != std::string::npos)	//检测是否缺省值
		{
			char tmpKeyValue[64] = { '\0' };
			std::string tmpValue_;
			//获取枚举字段名称
			str_enum_field_name = str_enum_field.substr(0, nPos);

			//解析枚举字段值
			tmpValue_ = str_enum_field.substr(nPos + 1, (*itor).length() - (nPos + 1));
			exp_value_list valueList;
			parse_exp_value(tmpValue_, valueList, tmp_stringKey);
			nEnumValue = parse_expression(valueList);
		}
		else
		{
			str_enum_field_name = str_enum_field;
			nEnumValue = nDefaultValue;
		}
		nDefaultValue = nEnumValue + 1;

		enumKeyValueList[nEnumValue] = str_enum_field_name;

		tmp_stringKey[str_enum_field_name] = nEnumValue;
	}
	all_enum_stringKey_list[enum_name] = tmp_stringKey;
	all_enum_map_list[enum_name] = enumKeyValueList;
	if (enumKeyValueList.size() == 0)
		return false;
	return true;
}

std::string TostringByValue(const std::string enum_name, const std::string cs_enum_info, const  int enum_value)
{
	enum_map_list::iterator itor = all_enum_map_list.find(enum_name);
	if (itor == all_enum_map_list.end())
	{
		enum_map enumKeyValueList;
		if (!parse_enum_str(enum_name, cs_enum_info, enumKeyValueList))
		{
			return std::string("");
		}
		itor = all_enum_map_list.find(enum_name);
		if (itor == all_enum_map_list.end())
			return  std::string("");
	}
	enum_map::iterator it = itor->second.find(enum_value);
	if (it == itor->second.end())
		return std::string("");
	else
		return it->second;
}
