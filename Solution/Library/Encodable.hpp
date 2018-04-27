#pragma once

#include <ostream>
#include <vector>
#include <string>

namespace SVDF
{

class Encodable
{

public:

	enum class Format
	{
		COMPACT,
		SPACE,
		NEWLINE,
		NEWLINE_TRUNCATE,
	};

	static void encode (std::ostream & stream, const std::vector<const Encodable *> & encodables, Format format = Format::NEWLINE);

	static std::string encode (const std::vector<const Encodable *> & encodables, Format format = Format::NEWLINE);

	virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const = 0;

	std::string encode (Format format = Format::NEWLINE) const;

	friend std::ostream & operator << (std::ostream & stream, const Encodable & encodable);
	
	virtual ~Encodable () = default;

};

}
