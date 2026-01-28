#ifndef _CCLASS_
#define _CCLASS_

#include <iostream>
#include <string>

using namespace std;

class CClass
{
	private:
		string destription;
		string number;
	public:
		string get_description();
		string get_number();
		void set_description(string);
		void set_number(string);
};

#endif
