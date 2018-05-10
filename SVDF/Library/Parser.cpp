#include "SVDF/Parser.hpp"

#include "SVDF/Grammar.hpp"
#include "SVDF/Utils.hpp"

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
			return consume_map ();
		}
		catch (Error &)
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
		return consume_comment ();
	}

	const std::istream & Parser::get_stream () const
	{
		return stream;
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
				consume ();
				consume_comment ();
				std::streampos pos = stream.tellg ();
				std::string key = consume_key ();
				if (map.has (key))
				{
					std::ostringstream ss;
					ss << "Error while parsing map. Duplicated key '";
					ss << str_utils::user_preview (key);
					ss << "'.";
					throw make_error_abs (ss.str (), pos);
				}
				consume_comment ();
				switch (pc)
				{
					case Grammar::string_entry_prefix:
					{
						std::string value;
						consume (Grammar::string_value_prefix);
						char c;
						while ((c = consume ()) != Grammar::string_value_suffix)
						{
							if (Grammar::is_string_char (c))
							{
								value += c;
								if (value.size () > Grammar::max_string_length)
								{
									std::ostringstream ss;
									ss << "Error while parsing map string entry value '";
									ss << str_utils::user_preview (value);
									ss << "'. std::string value size exceeds ";
									ss << Grammar::max_string_length;
									ss << " characters.";
									throw make_error_rel (ss.str (), -1);
								}
							}
							else
							{
								std::ostringstream ss;
								ss << "Illegal character '";
								ss << str_utils::inline_space (c);
								ss << "' encountered while parsing map string entry value.";
								throw make_error_rel (ss.str (), -1);
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
					std::ostringstream ss;
					ss << "Bad character '";
					ss << str_utils::inline_space (c);
					ss << "' encountered while parsing map." << std::endl;
					ss << "Expected map key prefixes '";
					ss << Grammar::string_entry_prefix;
					ss << "', '";
					ss << Grammar::int_entry_prefix;
					ss << "', or '";
					ss << Grammar::float_entry_prefix;
					ss << "', data prefix '";
					ss << Grammar::data_prefix;
					ss << "' or declaration suffix '";
					ss << Grammar::declaration_suffix;
					ss << "'.";
					throw make_error_rel (ss.str (), -1);
				}
			}
		}
	}

	std::string Parser::consume_key ()
	{
		std::string key;
		consume_comment ();
		while (Grammar::is_key_char (peek ()))
		{
			key += consume ();
			if (key.size () > Grammar::max_key_length)
			{
				std::ostringstream ss;
				ss << "Error while parsing map entry key '";
				ss << str_utils::user_preview (key);
				ss << "'." << std::endl;
				ss << "std::string size exceeds ";
				ss << Grammar::max_string_length;
				ss << " characters.";
				throw make_error_rel (ss.str (), -1);
			}
		}
		consume_comment ();
		consume (Grammar::key_suffix);
		return key;
	}

	void Parser::consume (char _c)
	{
		char c = consume ();
		if (c != _c)
		{
			std::ostringstream ss;
			ss << "Bad character '";
			ss << str_utils::inline_space (c);
			ss << "' encountered. Expected '";
			ss << _c;
			ss << "' character.";
			throw make_error_rel (ss.str (), -1);
		}
	}

	char Parser::consume ()
	{
		int c = stream.get ();
		if (c != EOF)
		{
			if (c > 0 && c < 256)
			{
				if (c == '\n')
				{
					state.current_line++;
					state.last_newline = stream.tellg ();
				}
				return static_cast<char>(c);
			}
			else
			{
				throw make_error_rel ("Unknown character encountered.", -1);
			}
		}
		else
		{
			throw make_error_rel ("Unexpected end of the stream.", 0);
		}
	}

	bool Parser::consume_comment ()
	{
		bool comment_block = false;
		while (true)
		{
			char c;
			if (!try_peek (c))
			{
				if (comment_block)
				{
					throw make_error_rel ("Unclosed comment at the end of the stream.", 0);
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (c == (comment_block ? Grammar::comment_suffix : Grammar::comment_prefix))
				{
					comment_block = !comment_block;
				}
				else if (!comment_block && !Grammar::is_space_char (c))
				{
					return true;
				}

				consume ();
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
			throw make_error_rel ("Unexpected end of the stream.", 0);
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
			if (c > 0 && c < 256)
			{
				_out = c;
				return true;
			}
			else
			{
				throw make_error_rel ("Unknown character encountered.", -1);
			}
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

	Parser::Error Parser::make_error_abs (const std::string & _message, std::streampos _pos)
	{
		int line{ state.current_line };
		int column{ Error::unknown_column };
		std::streamoff offset = _pos - state.last_newline;
		if (offset >= 0)
		{
			column = static_cast<int>(offset);
		}
		return Error{ _message, line, column };
	}

	Parser::Error Parser::make_error_rel (const std::string & _message, int _col_offs)
	{
		return make_error_abs (_message, stream.tellg () + static_cast<std::streampos>(_col_offs));
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
		std::ostringstream ss;
		ss << "Parse error occurred at line ";
		ss << (line + 1);
		if (column != Error::unknown_column)
		{
			ss << ", column ";
			ss << (column + 1);
		}
		ss << ":" << std::endl;
		ss << what ();
		return ss.str ();
	}

}
