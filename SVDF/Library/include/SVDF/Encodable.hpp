#pragma once

#include <ostream>
#include <sstream>
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

		template <typename T>
		static void encode (std::ostream & stream, const T & first, const T & last, Format format = Format::NEWLINE);

		template <typename T>
		static std::string encode (const T & first, const T & last, Format format = Format::NEWLINE);

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const = 0;

		std::string encode (Format format = Format::NEWLINE) const;

		friend std::ostream & operator << (std::ostream & stream, const Encodable & encodable);

		virtual ~Encodable () = default;

	};

	template <typename T>
	inline void Encodable::encode (std::ostream & stream, const T & first, const T & last, Format format)
	{
		for (auto it = first; it != last; ++it)
		{
			const Encodable * e = *it;
			if (it != first)
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
			e->encode (stream, format);
		}
	}

	template <typename T>
	inline std::string Encodable::encode (const T & first, const T & last, Format format)
	{
		std::ostringstream stream;
		encode (stream, first, last, format);
		return stream.str ();
	}

	template <typename T>
	std::ostream & operator << (std::ostream & stream, const std::vector<T> & vector)
	{
		Encodable::encode (stream, vector.begin (), vector.end ());
		return stream;
	}

}

