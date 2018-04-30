#pragma once

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

		inline bool is_space_char (char c)
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

		inline bool is_key_char (char c)
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

		inline bool is_string_char (char c)
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

}
