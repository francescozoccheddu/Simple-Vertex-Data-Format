#include "Declaration.hpp"
#include "Encodable.hpp"
#include "Grammar.hpp"
#include <ostream>

namespace SVDF
{

	bool SVDF::Declaration::has_data ()
	{
		return false;
	}

	void Declaration::encode (std::ostream & stream, Format format) const
	{
		map.encode (stream, format);
		switch (format)
		{
			case SVDF::Encodable::Format::SPACE:
			case SVDF::Encodable::Format::NEWLINE:
			case SVDF::Encodable::Format::NEWLINE_TRUNCATE:
				stream << " ";
				break;
			case SVDF::Encodable::Format::COMPACT:
				break;
		}
		stream << Grammar::declaration_suffix;
	}

}
