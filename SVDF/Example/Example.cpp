#include <iostream>
#include <vector>

#include "SVDF/Parser.hpp"
#include "SVDF/Declaration.hpp"
#include "SVDF/Comment.hpp"
#include "SVDF/Encodable.hpp"

using namespace SVDF;

int main ()
{
	FileParser parser{ "example.svdf" };

	if (parser.is_open ())
	{
		std::vector<Encodable*> document;

		document.push_back (new Comment{ "Some comment" });

		try
		{
			while (parser.has_declarations ())
			{
				Encodable * encodable = new DataDeclaration<float>{ parser.next_declaration<float> () };
				document.push_back (encodable);
			}
		}
		catch (Parser::Error & error)
		{
			std::cout << std::endl << error.where_and_what () << std::endl;
			std::cin.ignore ();
		}

		std::cout << "Re-encoded document:" << std::endl << std::endl;
		std::cout << document << std::endl;

		for (Encodable * encodable : document)
		{
			delete encodable;
		}
	}
	else
	{
		std::cout << "File not open" << std::endl;
	}

	std::cin.ignore ();
	return 0;
}
