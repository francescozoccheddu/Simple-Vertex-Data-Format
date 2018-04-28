
#include <string>
#include <iostream>
#include <fstream>
#include "../Library/Parser.hpp"
#include "../Library/Declaration.hpp"
#include "../Library/Comment.hpp"
#include <sstream>

using namespace SVDF;

int main ()
{

	FileParser p{ "../../example.svdf" };
	if (true)
	{
		std::vector<Declaration*> v;
		while (p.has_declarations ())
		{
			DataDeclaration<float> * d = new DataDeclaration<float>{ p.next_declaration<float>() };
			v.push_back (d);
		}

		Encodable::encode (std::cout, v.begin (), v.end ());

	}
	else
	{
		std::cout << "Not open" << std::endl;
	}

	system ("pause");
	return 0;
}