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

	Parser (std::istream & stream, int current_line, int current_col);
	
	template<typename T>
	bool next_number (T & out);

	bool next_delimiter (char & out);

	bool next_string (const char * alphabet, std::string & out);

private:

	bool consume_comment ();
	
	std::istream & stream;

	int last_newline;
	int current_line;
	
};

}

}