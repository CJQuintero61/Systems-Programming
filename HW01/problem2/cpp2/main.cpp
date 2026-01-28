#include "class.h"


int main()
{
	string desc = "Senior Capstone Project";
	string snum = "COSC4354";
		
	CClass aclass;
	
	aclass.set_description(desc);
	aclass.set_number(snum);


	cout << "Welocome to " << aclass.get_number() << ":" << aclass.get_description() << endl;
}
