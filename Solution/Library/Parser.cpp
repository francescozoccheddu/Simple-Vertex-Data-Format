#include "include/Parser.hpp"

#include "include/Grammar.hpp"

#include <string>
#include <cstdio>
#include <istream>

namespace SVDF
{

	Parser::Parser (std::istream & _stream) : _stream{ _stream }, state{ 0, 0, false, false }
	{}

	Map Parser::Parser::next_map ()
	{
		const BackupState backup{ make_backup () };
		try
		{
			Map map = consume_map ();
			char c = consume ();
			if (c == Grammar::data_prefix)
			{
				state.in_data = true;
				return map;
			}
			else if (c == Grammar::declaration_suffix)
			{
				return map;
			}
			else
			{
				throw std::logic_error ("Expected cosi");
			}
		}
		catch (std::logic_error &)
		{
			restore (backup);
			throw;
		}
	}

	bool Parser::has_data () const
	{
		return state.in_data;
	}

	bool Parser::is_compromised () const
	{
		return state.compromised;
	}

	bool Parser::has_declarations ()
	{
		if (_stream.good ())
		{
			consume_comment ();
			return !_stream.eof ();
		}
		else
		{
			return false;
		}
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
				_stream.get ();
				Key key = consume_key ();
				if (map.has (key))
				{
					throw std::logic_error ("Duplicate key");
				}
				consume_comment ();
				switch (pc)
				{
					case Grammar::string_entry_prefix:
					{
						String value;
						consume (Grammar::string_value_prefix);
						char c;
						while ((c = consume ()) != Grammar::string_value_suffix)
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
						_stream >> value;
						map.int_map.emplace (key, value);
					}
					break;
					case Grammar::float_entry_prefix:
					{
						float value;
						_stream >> value;
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

	Key Parser::consume_key ()
	{
		Key key;
		consume_comment ();
		while (Grammar::is_key_char (peek ()))
		{
			key += _stream.get ();
			if (key.size () > Grammar::max_key_length)
			{
				throw std::logic_error ("Name cannot be longer than max");
			}
		}
		consume_comment ();
		consume (Grammar::key_suffix);
		return key;
	}

	void Parser::consume (char _c)
	{
		if (consume () != _c)
		{
			throw std::logic_error ("Expected char c");
		}
	}

	char Parser::consume ()
	{
		int c = _stream.get ();
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
				_stream.get ();
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
						state.last_newline = _stream.tellg ();
					}
				}
				else
				{
					return;
				}
				_stream.get ();
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

	bool Parser::try_peek (char & _out)
	{
		int c = _stream.peek ();
		if (c == EOF)
		{
			return false;
		}
		else
		{
			_out = c;
			return true;
		}
	}

	Parser::BackupState Parser::make_backup () const
	{
		return BackupState{ state, _stream.tellg () };
	}

	void Parser::restore (const BackupState & _backup)
	{
		state.compromised = true;
		if (static_cast<int>(_backup.pos) != -1)
		{
			_stream.seekg (_backup.pos);
			state = _backup.state;
		}
	}

	FileParser::FileParser () : Parser{ file }
	{}

	FileParser::FileParser (const std::string & _filename) : file{ _filename }, Parser{ file }
	{}

	void FileParser::open (const std::string & _filename)
	{
		file.open (_filename);
	}

	bool FileParser::is_open () const
	{
		return file.is_open ();
	}

	void FileParser::close ()
	{
		file.close ();
	}

	StringParser::StringParser (const std::string & _string) : stringstream{ _string }, Parser{ stringstream }
	{}

}
