#include "include/Parser.hpp"

#include "include/Grammar.hpp"
#include "include/Utils.hpp"

#include <string>
#include <cstdio>
#include <cctype>
#include <istream>
#include <sstream>

namespace SVDF
{

	Parser::Parser (std::istream & _stream) : stream{ _stream }, state{ 0, 0, false, false }
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
				std::stringstream ss;
				ss << "Expected data prefix '";
				ss << Grammar::data_prefix;
				ss << "' or declaration suffix '";
				ss << Grammar::declaration_suffix;
				ss << "'";
				throw make_error (ss.str());
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
		if (stream.good ())
		{
			consume_comment ();
			return !stream.eof ();
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
				stream.get ();
				Key key = consume_key ();
				if (map.has (key))
				{
					std::stringstream ss;
					ss << "Duplicated key '";
					ss << str_utils::user_preview (key);
					ss << "'";
					throw make_error (ss.str());
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
								if (value.size () > Grammar::max_string_length)
								{
									std::stringstream ss;
									ss << "String value '";
									ss << str_utils::user_preview (value);
									ss << "' size exceeds ";
									ss << Grammar::max_string_length;
									ss << " characters";
									throw make_error (ss.str ());
								}
							}
							else
							{
								std::stringstream ss;
								ss << "Illegal character '";
								ss << str_utils::inline_space (c);
								ss << "' in string value";
								throw make_error (ss.str ());
							}
						}
						if (!value.is_valid ())
						{
							std::stringstream ss;
							ss << "Invalid key '";
							ss << str_utils::user_preview (key);
							ss << "'";
							throw make_error (ss.str ());
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

	Key Parser::consume_key ()
	{
		Key key;
		consume_comment ();
		while (Grammar::is_key_char (peek ()))
		{
			key += stream.get ();
			if (key.size () > Grammar::max_key_length)
			{
				std::stringstream ss;
				ss << "Key '";
				ss << str_utils::user_preview (key);
				ss << "' size exceeds ";
				ss << Grammar::max_string_length;
				ss << " characters";
				throw make_error (ss.str ());
			}
		}
		consume_comment ();
		consume (Grammar::key_suffix);
		if (!key.is_valid ())
		{
			std::stringstream ss;
			ss << "Invalid key '";
			ss << str_utils::user_preview (key);
			ss << "'";
			throw make_error (ss.str ());
		}
		return key;
	}

	void Parser::consume (char _c)
	{
		if (consume () != _c)
		{
			std::stringstream ss;
			ss << "Expected '";
			ss << _c;
			ss << "' character";
			throw make_error (ss.str ());
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
			throw make_error ("Unexpected end of stream");
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
					throw make_error ("Unclosed comment at end of stream");
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
			throw make_error ("Unexpected end of stream");
		}
	}

	bool Parser::try_peek (char & _out)
	{
		int c = stream.peek ();
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
		return BackupState{ state, stream.tellg () };
	}

	void Parser::restore (const BackupState & _backup)
	{
		state.compromised = true;
		if (static_cast<int>(_backup.pos) != -1)
		{
			stream.seekg (_backup.pos);
			state = _backup.state;
		}
	}

	Parser::Error Parser::make_error (const std::string _message)
	{
		int line{ state.current_line };
		int column{ Error::unknown_column };
		std::streamoff offset = stream.tellg () - state.last_newline;
		if (offset >= 0)
		{
			column = offset;
		}
		return Error{ _message, line, column };
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


	Parser::Error::Error (const std::string & _message, int _line, int _column) : std::runtime_error{ _message }, line{ _line }, column{ _column }, message{ make_message () }
	{}

	const std::string & Parser::Error::where_and_what () const
	{
		return message;
	}

	std::string Parser::Error::make_message () const
	{
		std::stringstream ss;
		ss << "Parse error occurred at line ";
		ss << line;
		if (column != Error::unknown_column)
		{
			ss << ", column ";
			ss << column;
		}
		ss << ":" << std::endl;
		ss << what ();
		return ss.str ();
	}

}
