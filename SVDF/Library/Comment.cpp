#include "SVDF/Comment.hpp"

#include "SVDF/Grammar.hpp"
#include "SVDF/Encodable.hpp"

#include <string>
#include <ostream>
#include <stdexcept>

namespace SVDF
{

	void SVDF::Comment::encode (std::ostream & _stream, Format _format) const
	{
		if (is_valid ())
		{

			if (!empty ())
			{
				bool space = false;

				switch (_format)
				{
					case Encodable::Format::COMPACT:
						break;
					case Format::SPACE:
					case Format::NEWLINE:
					case Format::NEWLINE_TRUNCATE:
						space = true;
						break;
				}

				_stream << Grammar::comment_prefix;
				if (space) { _stream << " "; }
				_stream << c_str ();
				if (space) { _stream << " "; }
				_stream << Grammar::comment_suffix;
			}
		}
		else
		{
			throw std::runtime_error ("Invalid comment");
		}
	}

	bool Comment::is_valid () const
	{
		return is_valid (*this);
	}

	bool Comment::is_valid (const std::string & comment)
	{
		for (char c : comment)
		{
			if (c == Grammar::comment_suffix)
			{
				return false;
			}
		}
		return true;
	}

}
