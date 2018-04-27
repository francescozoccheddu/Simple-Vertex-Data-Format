
#include <string>
#include <iostream>
#include <fstream>
#include "../Library/Parser.hpp"
#include "../Library/Declaration.hpp"

int main ()
{

	std::ifstream file ("../../example.svdf");
	if (file.is_open ())
	{
		SVDF::Parser p (file);
		while (!p.is_eof ())
		{
			SVDF::DataDeclaration<float> d{ p.next_declaration () };
			d.parse_data (p);
			std::cout << d << std::endl;
		}
	}
	else
	{
		std::cout << "Not open" << std::endl;
	}

	system ("pause");
	return 0;
}