#pragma once

#include "Grammar.hpp"
#include "Encodable.hpp"
#include "Map.hpp"
#include <ostream>

namespace SVDF
{

class Declaration : public Encodable
{

public:

	Map map;

	virtual bool has_data ();
	
	virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;
	
	virtual ~Declaration () = default;

};

}
