
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

	StringParser p{ "?name=\"ciao\";" };
	if (true)
	{
		std::vector<const Encodable * > v;
		v.push_back (new Comment{ "Prova" });
		while (p.has_declarations ())
		{
			DataDeclaration<float> * d = new DataDeclaration<float>{ p.next_declaration<float>() };
			v.push_back (d);
		}
		Encodable::encode (std::cout, v);
	}
	else
	{
		std::cout << "Not open" << std::endl;
	}

	system ("pause");
	return 0;
}