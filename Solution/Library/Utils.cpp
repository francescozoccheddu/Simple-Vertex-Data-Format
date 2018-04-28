#include "include/Utils.hpp"

#include <cctype>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <sstream>

namespace SVDF
{

	namespace str_utils
	{

		std::string user_preview (const std::string & _string)
		{
			static constexpr int length{ 10 };
			std::string str = _string.substr (0, length);
			if (str.size () == length)
			{
				str.resize (length);
				for (int d = length - 1; d > 0 && d > length - 1 - 3; d--)
				{
					str[d] = '.';
				}
			}
			for (int c = 0; c < str.size (); c++)
			{
				str[c] = inline_space (str[c]);
			}
			return str;
		}

		char inline_space (char _c)
		{
			return std::isspace (_c) ? ' ' : _c;
		}

	}

}
