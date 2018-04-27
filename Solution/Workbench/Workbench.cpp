
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
		while (!p.is_eof ())
		{
			p.next_declaration ();
			while (p.is_in_data_section ())
			{
				std::cout << p.next_value<float> () << ",";
			}
			std::cout << std::endl;
		}
	}
	else
	{
		std::cout << "Not open" << std::endl;
	}

	system ("pause");
	return 0;
}