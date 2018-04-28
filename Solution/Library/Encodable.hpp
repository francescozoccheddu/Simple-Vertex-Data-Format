#pragma once

#include <ostream>
#include <vector>
#include <string>
#include <type_traits>

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

	template <typename T>
	static void encode (std::ostream & stream, const std::vector<T*> & encodables, Format format = Format::NEWLINE);

	template <typename T>
	static std::string encode (const std::vector<T*> & encodables, Format format = Format::NEWLINE);

	virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const = 0;

	std::string encode (Format format = Format::NEWLINE) const;

	friend std::ostream & operator << (std::ostream & stream, const Encodable & encodable);
	
	virtual ~Encodable () = default;

};

template <typename T>
inline void Encodable::encode (std::ostream & stream, const std::vector<T*> &encodables, Format format)
{
	static_assert(std::is_base_of<Encodable, T>::value, "T must be Encodable");
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

template<typename T>
inline std::string Encodable::encode (const std::vector<T*>& encodables, Format format)
{
	std::ostringstream stream;
	Encodable::encode (stream, encodables, format);
	return stream.str ();
}

template<typename T, typename = std::enable_if_t<std::is_base_of<Encodable, T>::value>>
std::ostream & operator<<(std::ostream & stream, const std::vector<T*>& encodables)
{
	Encodable::encode (stream, encodables);
	return stream;
}

}
