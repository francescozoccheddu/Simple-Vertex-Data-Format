#include "include/Encodable.hpp"

#include <string>
#include <sstream>
#include <ostream>

namespace SVDF
{

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