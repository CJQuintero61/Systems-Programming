#include "class.h"


int main()
{
	string desc = "Operating Systems";
	string snum = "COSC3346";
		
	CClass aclass;
	
	aclass.set_description(desc);
	aclass.set_number(snum);


	cout << "Welocome to " << aclass.get_number() << ":" << aclass.get_description() << endl;
}
