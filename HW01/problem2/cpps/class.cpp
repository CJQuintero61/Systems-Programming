#include "class.h"


string CClass::get_description()
{

	return destription;
}

string CClass::get_number()
{

	return number;
}

void CClass:: set_description(string desc)
{
	destription = desc;		
}

void CClass::set_number(string name)
{
	number = name;
}
