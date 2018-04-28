#pragma once

#include "Encodable.hpp"

#include <string>

namespace SVDF
{

class Comment : public Encodable, public std::string
{

public:

	using std::string::string;
	
	virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

	bool is_valid () const;

	static bool is_valid (const std::string & comment);

};

}
