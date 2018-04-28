#pragma once

#include <ostream>
#include <vector>
#include <string>
#include <type_traits>
#include <iterator>
#include <list>

namespace SVDF
{

	class Encodable;

	namespace type_traits
	{

		template<typename T>
		struct is_encodable_pointer : std::is_base_of < Encodable, typename std::remove_pointer< T >::type > {};

		template<typename T>
		struct is_encodable_pointer_iterator : is_encodable_pointer < typename std::iterator_traits< T >::value_type > {};

	}

	template<typename T>
	using enable_if_encodable_pointer_t = typename std::enable_if<type_traits::is_encodable_pointer<T>::value>::type;

	template<typename T>
	using enable_if_encodable_pointer_iterator_t = typename std::enable_if<type_traits::is_encodable_pointer_iterator<T>::value>::type;

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

		template <typename Iterator, typename = typename enable_if_encodable_pointer_iterator_t<Iterator>>
		static void encode (std::ostream & stream, const Iterator & first, const Iterator & last, Format format = Format::NEWLINE)
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

		template <typename Iterator, typename = typename enable_if_encodable_pointer_iterator_t<Iterator>>
		static std::string encode (const Iterator & first, const Iterator & last, Format format = Format::NEWLINE)
		{
			std::ostringstream stream;
			encode (stream, first, last, format);
			return stream.str ();
		}

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const = 0;

		std::string encode (Format format = Format::NEWLINE) const;

		friend std::ostream & operator << (std::ostream & stream, const Encodable & encodable);

		virtual ~Encodable () = default;

	};

	template <typename T, typename = typename enable_if_encodable_pointer_t<T>>
	std::ostream & operator << (std::ostream & stream, const std::vector<T*> & collection)
	{
		Encodable::encode (stream, collection.begin(), collection.end());
		return stream;
	}

	template <typename T, typename = typename enable_if_encodable_pointer_t<T>>
	std::ostream & operator << (std::ostream & stream, const std::list<T*> & collection)
	{
		Encodable::encode (stream, collection.begin (), collection.end ());
		return stream;
	}

}
