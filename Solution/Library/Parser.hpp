#pragma once

#include "Map.hpp"
#include "Grammar.hpp"
#include "Declaration.hpp"

#include <string>
#include <fstream>
#include <istream>
#include <sstream>

namespace SVDF
{

	class Parser
	{

	public:

		Parser (std::istream & stream);

		Map next_map ();

		template<typename T>
		T next_value ();

		template<typename T>
		DataDeclaration<T> next_declaration ();

		bool has_data () const;

		bool is_compromised () const;

		bool has_declarations ();

		virtual ~Parser () = default;

	private:

		struct State
		{
			int current_line;
			std::streampos last_newline;
			bool compromised;
			bool in_data;
		};

		struct BackupState
		{
			State state;
			std::streampos pos;
		};

		std::istream & _stream;
		State state;

		Map consume_map ();

		Key consume_key ();

		void consume (char c);

		char consume ();

		void consume_comment ();

		char peek ();

		bool try_peek (char & out);

		BackupState make_backup () const;

		void restore (const BackupState & backup);

	};

	template<typename T>
	inline T Parser::next_value ()
	{
		_SVDF_ASSERT_DATA_VALUE_TYPE

		const BackupState backup{ make_backup () };
		try
		{
			consume_comment ();
			T val;
			_stream >> val;
			if (!_stream.fail ())
			{
				consume_comment ();
				char c = consume ();
				switch (c)
				{
					case Grammar::value_separator:
						break;
					case Grammar::declaration_suffix:
						state.in_data = false;
						break;
					default:
						throw std::logic_error ("Expected value separator or data suffix");
				}
				return val;
			}
			else
			{
				throw std::logic_error ("Value parse failed");
			}
		}
		catch (std::logic_error &)
		{
			restore (backup);
			throw;
		}
	}

	template<typename T>
	inline DataDeclaration<T> Parser::next_declaration ()
	{
		_SVDF_ASSERT_DATA_VALUE_TYPE

		DataDeclaration<T> d{ next_map () };
		d.parse_data (*this);
		return d;
	}

	class FileParser : public Parser
	{

	public:

		FileParser ();

		FileParser (const std::string & _filename);

		void open (const std::string & _filename);

		bool is_open () const;

		void close ();

	private:

		std::ifstream file;
	
	};

	class StringParser : public Parser
	{

	public:

		StringParser (const std::string & string);

	private:

		std::istringstream stringstream;

	};

	// TODO Exceptions

}