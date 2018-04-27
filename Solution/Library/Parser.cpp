#include "Parser.hpp"

#include <cstdio>
#include <istream>

SVDF::Parser::Parser (std::istream & _stream) : stream{ _stream }, state{ 0, 0, false, false, false }
{}

SVDF::ListInfo SVDF::Parser::Parser::next_list ()
{
	// Header
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
	consume (Grammar::name_prefix);
	consume_comment ();
	ListInfo info;
	// Name
	{
		char c;
		while (Grammar::is_name_char (c = peek ()) && info.name.size () <= Grammar::max_name_length)
		{
			stream.get ();
			info.name += c;
		}
		if (info.name.size () < 1)
		{
			throw std::logic_error ("Expected name");
		}
		else if (info.name.size () > Grammar::max_name_length)
		{
			throw std::logic_error ("Name cannot be longer than max");
		}
	}
	consume_comment ();
	// Length
	{
		char c = peek ();
		if (c == Grammar::length_prefix)
		{
			stream.get ();
			consume_comment ();
			int l;
			stream >> l;
			if (!stream.fail () && l >= 0)
			{
				info.length = l;
			}
			else
			{
				throw std::logic_error ("Expected natural");
			}
		}
		else
		{
			info.length = ListInfo::no_lenght;
		}
	}
	consume_comment ();
	consume (Grammar::data_prefix);
	consume_comment ();
	state.data_section = true;
	return info;
}

bool SVDF::Parser::is_in_data_section () const
{
	return state.data_section;
}

bool SVDF::Parser::is_compromised () const
{
	return state.compromised;
}

bool SVDF::Parser::is_eof () const
{
	return stream.eof ();
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
	if (c != EOF)
	{
		return static_cast<char>(c);
	}
	else
	{
		throw std::logic_error ("Unexpected EOF");
	}
}

void SVDF::Parser::consume_comment ()
{
	bool comment_block = false;
	while (true)
	{
		char c;
		if (!try_peek (c))
		{
			stream.get ();
			if (comment_block)
			{
				throw std::runtime_error ("Unclosed comment block at EOF");
			}
			else
			{
				return;
			}
		}
		else
		{
			if (c == (comment_block ? Grammar::comment_suffix : Grammar::comment_prefix))
			{
				comment_block = !comment_block;
			}
			else if (comment_block || Grammar::is_space_char (c))
			{
				if (c == '\n')
				{
					state.current_line++;
					state.last_newline = stream.tellg ();
				}
			}
			else
			{
				return;
			}
			stream.get ();
		}
	}
}

char SVDF::Parser::peek ()
{
	char c;
	if (try_peek (c))
	{
		return c;
	}
	else
	{
		throw std::logic_error ("Unexpected EOF");
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
