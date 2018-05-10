#pragma once

#include "Encodable.hpp"

#include <string>
#include <map>
#include <ostream>

namespace SVDF
{

	class Parser;

	class Map : public Encodable
	{

	public:

		enum class Type
		{
			STRING, INT, FLOAT
		};

		static bool is_key_valid (const std::string & key);

		static bool is_string_valid (const std::string & string);

		bool get_string (const std::string & key, std::string & out) const;

		bool get_int (const std::string & key, int & out) const;

		bool get_float (const std::string & key, float & out) const;

		bool get_type (const std::string & key, Type & out) const;

		bool has (const std::string & key) const;

		bool put_string (const std::string & key, const std::string & value);

		bool put_int (const std::string & key, int value);

		bool put_float (const std::string & key, float value);

		bool remove (const std::string & key);

		const std::map<std::string, std::string> & get_string_map () const;

		const std::map<std::string, int> & get_int_map () const;

		const std::map<std::string, float> & get_float_map () const;

		void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

	private:

		friend class Parser;

		std::map<std::string, std::string> string_map;
		std::map<std::string, int> int_map;
		std::map<std::string, float> float_map;

	};

}

