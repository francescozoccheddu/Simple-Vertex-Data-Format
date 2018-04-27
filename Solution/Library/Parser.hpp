#pragma once

#include "Map.hpp"
#include "Grammar.hpp"
#include <istream>
#include <type_traits>

namespace SVDF
{

	class Parser
	{

	public:

		Parser (std::istream & stream);

		Map next_declaration ();

		template<typename T>
		T next_value ();

		bool is_in_data_section () const;

		bool is_compromised () const;

		bool is_eof ();

	private:

		struct State
		{
			int current_line;
			std::streampos last_newline;
			bool compromised;
			bool data_section;
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

		void consume (const std::string & string);

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
		static_assert(std::is_arithmetic<T>::value, "T must be numeric type");
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
						state.data_section = false;
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

}