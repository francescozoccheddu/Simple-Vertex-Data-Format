#pragma once

#include <istream>

namespace SVDF
{

namespace Parser
{

class Parser
{

public:

	Parser (std::istream & stream);
	
	template<typename T>
	bool next_number (T & out);

	bool next_delimiter (char & out);

	std::string next_string (const char * alphabet);

	int consume_string (const std::string & string);

private:

	bool consume_comment ();
	
	std::istream & stream;

	int last_newline;
	int current_line;
	
};

}

}