#pragma once

#include <ostream>
#include <vector>
#include <string>
#include <type_traits>
#include <iterator>

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

		template <typename Iterator, typename = enable_if_iterator<Iterator>::type>
		static void encode (std::ostream & stream, const Iterator & first, const Iterator & last, Format format = Format::NEWLINE)
		{
			Encodable::EncodableIterator<Iterator> (first, last).encode (stream, format);
		}

		template <typename Iterator, typename = enable_if_iterator<Iterator>::type>
		static std::string encode (const Iterator & first, const Iterator & last, Format format = Format::NEWLINE)
		{
			return Encodable::EncodableIterator<Iterator> (first, last).encode (format);
		}

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const = 0;

		std::string encode (Format format = Format::NEWLINE) const;

		friend std::ostream & operator << (std::ostream & stream, const Encodable & encodable);

		virtual ~Encodable () = default;

	private:

		template<class T, typename P = std::iterator_traits<T>::value_type, typename V = std::remove_pointer< P >::type >
		struct enable_if_iterator : std::enable_if< std::is_base_of <Encodable, V >::value > {};

		template <typename Iterator, typename = enable_if_iterator<Iterator>::type>
		class EncodableIterator;

	};

	template <typename Iterator, typename >
	class Encodable::EncodableIterator : public Encodable
	{

	public:

		EncodableIterator (const Iterator & begin, const Iterator & end) : begin{ begin }, end{ end } {}

		void encode (std::ostream & stream, Format format = Format::NEWLINE) const override
		{
			for (auto it = begin; it != end; ++it)
			{
				const Encodable * e = *it;
				if (it != begin)
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

	private:

		Iterator begin;
		Iterator end;

	};




}
