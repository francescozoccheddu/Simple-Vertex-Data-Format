#pragma once

#include "Grammar.hpp"
#include "Parser.hpp"
#include "Encodable.hpp"
#include "Map.hpp"
#include <ostream>
#include <utility>

namespace SVDF
{

	class Declaration : public Encodable
	{

	public:

		Declaration () = default;

		Declaration (const Map & map);

		Declaration (const Map && map);

		Map map;

		virtual bool has_data () const;

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

		virtual ~Declaration () = default;

	};

	template<typename T>
	class DataDeclaration : public Declaration
	{

	public:

		DataDeclaration () = default;

		DataDeclaration (const Declaration & declaration);

		DataDeclaration (const Declaration && declaration);

		std::vector<T> data;

		bool has_data () const override;

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

		void parse_data (Parser & parser);

	};

	template<typename T>
	inline DataDeclaration<T>::DataDeclaration (const Declaration & declaration) : Declaration{ declaration.map }
	{}

	template<typename T>
	inline DataDeclaration<T>::DataDeclaration (const Declaration && declaration) : Declaration{ std::move (declaration.map) }
	{}

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
		while (parser.is_in_data_section ())
		{
			data.push_back (parser.next_value<T> ());
		}
	}

}