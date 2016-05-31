#include "enum_map.h"
#include <stdio.h>
#include <list>

enum_map_list all_enum_map_list;

typedef std::map<std::string, int> enum_stringKey;
typedef std::map<std::string, enum_stringKey>	enum_map_stringKey_list;
enum_map_stringKey_list all_enum_stringKey_list;

//���������
enum exp_type
{
	add_,		//��
	sub_,		//��
	mul_,		//��
	exc_,		//��
	lmove_,		//����
	rmove_,		//����
	lbracket_,	//������
	rbracket_,	//������
	moreThan_,	//����
	lessThan_,	//С��
	value_type_count,
};



//����ֵ
struct str_valueType
{
	int  nValue;
	bool bIsExpOpt;
	bool isExpOpt(){ return bIsExpOpt; }
	void setValue(int value_){ nValue = value_; bIsExpOpt = false; }
	void setOpt(int value_) { nValue = value_; bIsExpOpt = true; }
	int		GetValue(){ return nValue; }
};
typedef std::list<str_valueType> exp_value_list;	//���ʽ����ֵ list

//��ȡ��������
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
	if (str.find("0x") != std::string::npos)	//16����
		nEnumValue = strtol(str.c_str(), NULL, 16);
	else if (str[0] == '0')//8����
		nEnumValue = strtol(str.c_str(), NULL, 8);
	else
	{
		//����Ƿ���ö��ֵ
		bool bEnum = false;
		for (int i = 0; i < str.length(); i++)
		{
			if ('0' > str[i] || str[i] > '9')
			{
				bEnum = true;
				break;
			}
		}
		if (bEnum)	//ö��ֵ
		{
			//���ȴ�����ö�ٵ� ö��ֵ�б����ȡ
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
			nEnumValue = strtol(str.c_str(), NULL, 10);//10����
	}
	return nEnumValue;
}
//����
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

//����������ʽ��ֵ ���������
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
//ȥ�ո�
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
//enum ֵ�ı��ʽĿǰֻ֧�� + - * / <<  >>  ����������
//�������ʽ
int parse_expression(exp_value_list& valueList)
{
	if (valueList.size() == 0)
		return 0;
	if (valueList.size() == 1)
		return valueList.begin()->GetValue();

	//�Ƚ���������ȼ� ���� 
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
				//һ�������ڵ�������ʽ �ݹ����
				exp_value_list _tmp_list;
				breacket_begin++;
				_tmp_list.assign(breacket_begin, breacket_end);
				int nValue_ = parse_expression(_tmp_list);

				//ɾ�������ڵı��ʽ
				breacket_begin--;
				breacket_end++;
				itor = valueList.erase(breacket_begin, breacket_end);

				//����������ֵ��ԭ��ָ��λ��
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


	//������ʽ���� ---ǰ�����ã������㷨�Ľ�
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
	//����ö�ٶ��ŷָ���
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

	//��������ö��ֵ��Ӧ��������ֵ
	enum_stringKey tmp_stringKey;
	enumKeyValueList.clear();
	int nDefaultValue = 0;
	for (std::vector<std::string>::iterator itor = enum_value_list.begin(); itor != enum_value_list.end(); itor++)
	{
		std::string str_enum_field = earse_spel(*itor);	//ȥ�ո�
		long			nEnumValue;
		std::string str_enum_field_name;

		//������ö��ֵ��Ӧ��������ֵ
		int nPos = str_enum_field.find("=");
		if (nPos != std::string::npos)	//����Ƿ�ȱʡֵ
		{
			char tmpKeyValue[64] = { '\0' };
			std::string tmpValue_;
			//��ȡö���ֶ�����
			str_enum_field_name = str_enum_field.substr(0, nPos);

			//����ö���ֶ�ֵ
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
