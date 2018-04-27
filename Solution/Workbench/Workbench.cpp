
#include <string>
#include <iostream>
#include <fstream>
#include "../Library/Parser.hpp"

int main ()
{
	std::ifstream file ("../../example.svdf");
	if (file.is_open ())
	{
		SVDF::Parser p (file);
		p.next_list ();
	}
	else
	{
		std::cout << "Not open" << std::endl;
	}

	system ("pause");
	return 0;
}