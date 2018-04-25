
#include <string>
#include <iostream>
#include "../Library/ImmediatePointer.hpp"

int main ()
{

	Pointer<int,float> * p = new ImmediatePointer<int, float, true> { 3,3 };
	Pointer<int,float> * p2 = new ImmediatePointer<int, float, false> { 3,3 };

	std::cout << p->is_real () << std::endl;
	std::cout << p2->is_real () << std::endl;

	system ("pause");
	return 0;
}