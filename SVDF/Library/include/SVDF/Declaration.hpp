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

	public:

		using Declaration::Declaration;

		DataDeclaration () = default;

		DataDeclaration (const DataDeclaration<T> & declaration) = default;

		DataDeclaration (DataDeclaration<T> && declaration) = default;

		std::vector<T> data{};

		bool has_data () const override;

		virtual void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

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

}
