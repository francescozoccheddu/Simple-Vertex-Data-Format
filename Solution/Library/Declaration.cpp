#include "include/Declaration.hpp"

#include "include/Encodable.hpp"
#include "include/Grammar.hpp"

#include <ostream>
#include <utility>

namespace SVDF
{

	Declaration::Declaration (const Map & _map) : map{ _map }
	{}

	Declaration::Declaration (Map && _map) : map{ std::move (_map) }
	{}

	bool Declaration::has_data () const
	{
		return false;
	}

	void Declaration::encode (std::ostream & _stream, Format _format) const
	{
		map.encode (_stream, _format);
		switch (_format)
		{
			case Format::SPACE:
			case Format::NEWLINE:
			case Format::NEWLINE_TRUNCATE:
				_stream << " ";
				break;
			case Encodable::Format::COMPACT:
				break;
		}
		_stream << Grammar::declaration_suffix;
	}

}
