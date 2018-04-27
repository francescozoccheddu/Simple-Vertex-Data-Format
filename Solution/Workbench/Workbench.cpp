
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
		SVDF::ListInfo i = p.next_list ();
		std::cout << "Name: " << std::endl << i.name << std::endl;
		std::cout << "Length: " << std::endl << i.length << std::endl;
	}
	else
	{
		std::cout << "Not open" << std::endl;
	}

	system ("pause");
	return 0;
}