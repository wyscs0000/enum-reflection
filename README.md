# enum-reflection
enumeration reflection for C++



//I wrote a macro to define enumeration ,and macro name : enum_define 

example:

//define enum 

enum_define(	_myEnum, 	//head field is enum name
		first_field,	// The real field  from here  ,default value : 0
		second_field = 10,
		third_field = (second_field << 5) + 2, // Currently supported operators: + ,- ,* ,/ ,() ,<< ,>>	
		fourth_field = 0x22,
	)

	
int main()
{
	/** this function is used to retrieve the field names.
	*	@enum_name : the enumeration name
	*	@enum_value_int : the field value
	*	return	 the field name by string
	*	std::string toStringByEnumValue(enum_name,enum_value_int);
	*/

	printf("%s\n", toStringByEnumValue(_myEnum, 0).c_str());
	printf("%s\n", toStringByEnumValue(_myEnum, 10).c_str());
	printf("%s\n", toStringByEnumValue(_myEnum, 0x22).c_str());
	return 0;	
}
