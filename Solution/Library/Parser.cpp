#include "Parser.hpp"

#include <cstdio>
#include <istream>

SVDF::Parser::Parser (std::istream & _stream) : stream{ _stream }, state{ 0, 0, false, false, false }
{}

SVDF::ListInfo SVDF::Parser::Parser::next_list ()
{
	if (!state.header_consumed)
	{
		consume_comment ();
		consume (Grammar::header_prefix);
		consume_comment ();
		consume (Grammar::header);
		consume_comment ();
		consume (Grammar::header_suffix);
		state.header_consumed = true;
	}
	consume_comment ();
	return ListInfo{};
}

void SVDF::Parser::skip_value ()
{}

bool SVDF::Parser::is_in_data_section () const
{
	return false;
}

bool SVDF::Parser::is_compromised () const
{
	return false;
}

void SVDF::Parser::consume_comment ()
{
	bool comment_block = false;
	while (true)
	{
		char c;
		if (!try_peek (c))
		{
			if (comment_block)
			{
				throw std::runtime_error ("Unclosed comment block at EOF");
			}
			return;
		}
		else
		{
			if (c == (comment_block ? Grammar::comment_suffix : Grammar::comment_prefix))
			{
				comment_block = !comment_block;
			}
			else if (!comment_block && !Grammar::is_space_char (c))
			{
				return;
			}
			stream.get ();
			if (c == '\n')
			{
				state.current_line++;
				state.last_newline = stream.tellg ();
			}
		}
	}

}

void SVDF::Parser::consume (const std::string & string)
{
	for (char c : string)
	{
		consume (c);
	}
}

void SVDF::Parser::consume (char c)
{
	if (consume () != c)
	{
		throw std::logic_error ("Expected char c");
	}
}

char SVDF::Parser::consume ()
{
	int c = stream.get ();
	if (c == EOF)
	{
		throw std::logic_error ("Unexpected EOF");
	}
	else
	{
		return static_cast<char>(c);
	}
}

bool SVDF::Parser::try_peek (char & out)
{
	int c = stream.peek ();
	if (c == EOF)
	{
		return false;
	}
	else
	{
		out = c;
		return true;
	}
}

SVDF::Parser::BackupState SVDF::Parser::backup () const
{
	return BackupState{ state, stream.tellg () };
}

void SVDF::Parser::restore (BackupState backup)
{
	state.compromised = true;
	if (static_cast<int>(backup.pos) != -1)
	{
		stream.seekg (backup.pos);
		state = backup.state;
	}
}
