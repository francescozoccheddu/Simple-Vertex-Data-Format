#pragma once

#include "Grammar.hpp"
#include "Encodable.hpp"
#include "Map.hpp"

#include <type_traits>
#include <ostream>
#include <utility>

namespace SVDF
{

	template<typename> struct is_data_value_base : std::false_type {};

	template<> struct is_data_value_base<long double> : std::true_type {};
	template<> struct is_data_value_base<double> : std::true_type {};
	template<> struct is_data_value_base<float> : std::true_type {};
	template<> struct is_data_value_base<signed long long> : std::true_type {};
	template<> struct is_data_value_base<unsigned long long> : std::true_type {};
	template<> struct is_data_value_base<signed long> : std::true_type {};
	template<> struct is_data_value_base<unsigned long> : std::true_type {};
	template<> struct is_data_value_base<signed int> : std::true_type {};
	template<> struct is_data_value_base<unsigned int> : std::true_type {};
	template<> struct is_data_value_base<signed short> : std::true_type {};
	template<> struct is_data_value_base<unsigned short> : std::true_type {};

	template<typename T> struct is_data_value : is_data_value_base<std::remove_volatile_t<T>> {};

#define _SVDF_ASSERT_DATA_VALUE_TYPE static_assert(SVDF::is_data_value<T>::value, "T must be a non-const fundamental numeric type");

	class Parser;

	class Declaration : public Encodable
	{

	public:

		Declaration () = default;

		Declaration (const Map & map);

		Declaration (Map && map);

		Declaration (const Declaration & declaration) = default;

		Declaration (Declaration && declaration) = default;

		Map map;

		virtual bool has_data () const;

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

		virtual ~Declaration () = default;

	};

	template<typename T>
	class DataDeclaration : public Declaration
	{
		_SVDF_ASSERT_DATA_VALUE_TYPE

	public:

		using Declaration::Declaration;

		DataDeclaration (const DataDeclaration & declaration) = default;

		DataDeclaration (DataDeclaration && declaration) = default;

		std::vector<T> data;

		bool has_data () const override;

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

		void parse_data (Parser & parser);

	};

	template<typename T>
	inline bool DataDeclaration<T>::has_data () const
	{
		return !data.empty ();
	}

	template<typename T>
	inline void DataDeclaration<T>::encode (std::ostream & stream, Format format) const
	{
		if (has_data ())
		{
			map.encode (stream, format);
			switch (format)
			{
				case SVDF::Encodable::Format::COMPACT:
					break;
				case SVDF::Encodable::Format::SPACE:
					stream << " ";
					break;
				case SVDF::Encodable::Format::NEWLINE:
				case SVDF::Encodable::Format::NEWLINE_TRUNCATE:
					stream << "\n";
					break;
			}
			stream << Grammar::data_prefix;
			switch (format)
			{
				case SVDF::Encodable::Format::COMPACT:
					break;
				case SVDF::Encodable::Format::SPACE:
				case SVDF::Encodable::Format::NEWLINE:
				case SVDF::Encodable::Format::NEWLINE_TRUNCATE:
					stream << " ";
					break;
			}
			int vi = 0;
			for (T v : data)
			{
				if (vi != 0)
				{
					stream << Grammar::value_separator;
					switch (format)
					{
						case SVDF::Encodable::Format::COMPACT:
							break;
						case SVDF::Encodable::Format::SPACE:
						case SVDF::Encodable::Format::NEWLINE:
							stream << " ";
							break;
						case SVDF::Encodable::Format::NEWLINE_TRUNCATE:
							stream << (vi % 10 == 0) ? "\n" : " ";
							break;
					}
				}
				stream << v;
				vi++;
			}
			stream << Grammar::declaration_suffix;
		}
		else
		{
			Declaration::encode (stream, format);
		}

	}

	template<typename T>
	inline void DataDeclaration<T>::parse_data (Parser & parser)
	{
		while (parser.has_data ())
		{
			data.push_back (parser.next_value<T> ());
		}
	}

}