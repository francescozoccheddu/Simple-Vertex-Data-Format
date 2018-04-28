
#include <iostream>
#include <vector>

#include "../Library/include/Parser.hpp"
#include "../Library/include/Declaration.hpp"
#include "../Library/include/Comment.hpp"
#include "../Library/include/Encodable.hpp"

using namespace SVDF;

int main ()
{
	FileParser p{ "example.svdf" };

	if (p.is_open())
	{
		std::vector<Encodable*> document;

		document.push_back (new Comment{ "Some comment" });

		while (p.has_declarations ())
		{
			try
			{
				DataDeclaration<float> * d = new DataDeclaration<float>{ p.next_declaration<float>() };
				document.push_back (d);
			}
			catch (int)
			{

			}
		}

		std::cout << "Re-encoded document:" << std::endl << std::endl;
		std::cout << document << std::endl;

		for (Encodable * e : document)
		{
			delete e;
		}
	}
	else
	{
		std::cout << "File not open" << std::endl;
	}

	std::cin.ignore ();
	return 0;
}