#pragma once

#include <istream>
#include <type_traits>

namespace SVDF
{

namespace Grammar
{
	constexpr char header_prefix{ '[' };
	constexpr char header[]{ "SVDFv0.1" };
	constexpr char header_suffix{ ']' };
	constexpr char name_prefix{ '@' };
	constexpr char length_prefix{ '#' };
	constexpr char data_prefix{ '=' };
	constexpr char data_separator{ ',' };
	constexpr char data_suffix{ ';' };
	constexpr char comment_prefix{ '<' };
	constexpr char comment_suffix{ '>' };
	constexpr int max_name_length{ 512 };

	constexpr bool is_space_char (char c)
	{
		constexpr char space_alphabet[]{ " \t\n\v\f\r" };
		for (char space_char : space_alphabet)
		{
			if (space_char == c)
			{
				return true;
			}
		}
		return false;
	}

	constexpr bool is_name_char (char c)
	{
		constexpr char name_alphabet[]{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_!$%&?." };
		for (char name_char : name_alphabet)
		{
			if (name_char == c)
			{
				return true;
			}
		}
		return false;
	}

}


struct ListInfo
{
	static constexpr int no_lenght{ -1 };
	std::string name;
	int length;
};

class Parser
{

public:

	Parser (std::istream & stream);
	
	ListInfo next_list ();

	template<typename T>
	T next_value ();

	bool is_in_data_section () const;

	bool is_compromised () const;

	bool is_eof () const;

private:

	struct State
	{
		int current_line;
		std::streampos last_newline;
		bool compromised;
		bool header_consumed;
		bool data_section;
	};

	struct BackupState
	{
		State state;
		std::streampos pos;
	};

	std::istream & stream;
	State state;

	void consume (const std::string& string);

	void consume (char c);

	char consume ();

	void consume_comment ();

	char peek ();

	bool try_peek (char & out);

	BackupState backup () const;

	void restore (BackupState backup);
	
};

template<typename T>
inline T Parser::next_value ()
{
	static_assert(std::is_arithmetic<T>::value, "T must be numeric type");
	return T ();
}

}