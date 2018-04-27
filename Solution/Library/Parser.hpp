#pragma once

#include "Map.hpp"
#include <istream>
#include <type_traits>

namespace SVDF
{

	namespace Grammar
	{
		constexpr char string_entry_prefix{ '?' };
		constexpr char string_value_prefix{ '"' };
		constexpr char string_value_suffix{ '"' };
		constexpr char int_entry_prefix{ '#' };
		constexpr char float_entry_prefix{ '$' };
		constexpr char data_prefix{ ':' };
		constexpr char key_suffix{ '=' };
		constexpr char value_separator{ ',' };
		constexpr char declaration_suffix{ ';' };
		constexpr char comment_prefix{ '<' };
		constexpr char comment_suffix{ '>' };
		constexpr int max_key_length{ 512 };
		constexpr int max_string_length{ 512 };
		constexpr char space_alphabet[]{ " \t\n\v\f\r" };
		constexpr char key_alphabet[]{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_." };
		constexpr char string_alphabet[]{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.,!?^$%&/()[]{}+-*\\ " };

		constexpr bool is_space_char (char c)
		{
			for (char space_char : space_alphabet)
			{
				if (space_char == c)
				{
					return true;
				}
			}
			return false;
		}

		constexpr bool is_key_char (char c)
		{
			for (char key_char : key_alphabet)
			{
				if (key_char == c)
				{
					return true;
				}
			}
			return false;
		}

		constexpr bool is_string_char (char c)
		{
			for (char string_char : string_alphabet)
			{
				if (string_char == c)
				{
					return true;
				}
			}
			return false;
		}

	}

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

		std::istream & stream;
		State state;

		Map consume_map ();

		std::string consume_key ();

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
			stream >> val;
			if (!stream.fail ())
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