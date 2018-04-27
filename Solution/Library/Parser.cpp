#include "Parser.hpp"

#include <string>
#include <cstdio>
#include <istream>

namespace SVDF
{

	Parser::Parser (std::istream & _stream) : stream{ _stream }, state{ 0, 0, false, false }
	{}

	Map Parser::Parser::next_declaration ()
	{
		const BackupState backup{ make_backup () };
		try
		{
			/*consume_comment ();
			std::string name;
			{
				char c;
				while (Grammar::is_key_char (c = peek ()) && name.size () <= Grammar::max_name_length)
				{
					stream.get ();
					name += c;
				}
				if (name.size () < 1)
				{
					throw std::logic_error ("Expected name character");
				}
				else if (name.size () > Grammar::max_name_length)
				{
					throw std::logic_error ("Name cannot be longer than max");
				}
			}
			consume_comment ();
			consume (Grammar::data_prefix);
			state.data_section = true;
			return name;*/
			return Map{};
		}
		catch (std::logic_error &)
		{
			restore (backup);
			throw;
		}
	}

	bool Parser::is_in_data_section () const
	{
		return state.data_section;
	}

	bool Parser::is_compromised () const
	{
		return state.compromised;
	}

	bool Parser::is_eof ()
	{
		consume_comment ();
		return stream.eof ();
	}

	Map Parser::consume_map ()
	{
		Map map;
		while (true)
		{
			consume_comment ();
			char pc = peek ();
			if (pc == Grammar::string_entry_prefix || pc == Grammar::int_entry_prefix || pc == Grammar::float_entry_prefix)
			{
				stream.get ();
				std::string key = consume_key ();
				if (map.has (key))
				{
					throw std::logic_error ("Duplicate key");
				}
				consume_comment ();
				switch (pc)
				{
					case Grammar::string_entry_prefix:
					{
						std::string value;
						consume (Grammar::string_entry_prefix);
						char c;
						while ((c = consume()) != Grammar::string_value_suffix)
						{
							if (Grammar::is_string_char (c))
							{
								value += c;
								if (key.size () > Grammar::max_string_length)
								{
									throw std::logic_error ("String cannot be longer than max");
								}
							}
							else
							{
								throw std::logic_error ("Illegal character in string value");
							}
						}
						map.string_map.emplace (key, value);
					}
					break;
					case Grammar::int_entry_prefix:
					{
						int value;
						stream >> value;
						map.int_map.emplace (key, value);
					}
					break;
					case Grammar::float_entry_prefix:
					{
						float value;
						stream >> value;
						map.float_map.emplace (key, value);
					}
					break;
				}
			}
			else
			{
				return map;
			}
		}
	}

	std::string Parser::consume_key ()
	{
		std::string key;
		consume_comment ();
		while (Grammar::is_key_char (peek ()))
		{
			key += stream.get ();
			if (key.size () > Grammar::max_key_length)
			{
				throw std::logic_error ("Name cannot be longer than max");
			}
		}
		consume_comment ();
		consume (Grammar::key_suffix);
		return key;
	}

	void Parser::consume (const std::string & string)
	{
		for (char c : string)
		{
			consume (c);
		}
	}

	void Parser::consume (char c)
	{
		if (consume () != c)
		{
			throw std::logic_error ("Expected char c");
		}
	}

	char Parser::consume ()
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

	void Parser::consume_comment ()
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

	char Parser::peek ()
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

	bool Parser::try_peek (char & out)
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

	Parser::BackupState Parser::make_backup () const
	{
		return BackupState{ state, stream.tellg () };
	}

	void Parser::restore (const BackupState & backup)
	{
		state.compromised = true;
		if (static_cast<int>(backup.pos) != -1)
		{
			stream.seekg (backup.pos);
			state = backup.state;
		}
	}

}
