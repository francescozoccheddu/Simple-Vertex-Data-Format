#include "Encodable.hpp"

#include <string>
#include <sstream>
#include <ostream>

namespace SVDF
{

	void Encodable::encode (std::ostream & stream, const std::vector<const Encodable*> &encodables, Format format)
	{
		bool first = true;
		for (const Encodable * e : encodables)
		{
			if (!first)
			{
				switch (format)
				{
					case SVDF::Encodable::Format::COMPACT:
						break;
					case SVDF::Encodable::Format::SPACE:
						stream << "\n";
						break;
					case SVDF::Encodable::Format::NEWLINE:
					case SVDF::Encodable::Format::NEWLINE_TRUNCATE:
						stream << "\n\n";
						break;
				}
			}
			first = false;
			e->encode (stream, format);
		}
	}

	std::string Encodable::encode (const std::vector<const Encodable*>& encodables, Format format)
	{
		std::ostringstream stream;
		Encodable::encode (stream, encodables, format);
		return stream.str ();
	}

	std::string Encodable::encode (Format format) const
	{
		std::ostringstream stream;
		encode (stream, format);
		return stream.str ();
	}

	std::ostream & operator<<(std::ostream & stream, const Encodable & encodable)
	{
		encodable.encode (stream);
		return stream;
	}

}