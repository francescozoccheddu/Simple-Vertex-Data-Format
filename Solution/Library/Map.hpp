#pragma once

#include <string>
#include <map>

namespace SVDF
{

	class Parser;

	class Map
	{

	public:

		enum class Type
		{
			STRING, INT, FLOAT
		};

		bool get_string (const std::string & key, std::string & out) const;

		bool get_int (const std::string & key, int & out) const;

		bool get_float (const std::string & key, float & out) const;

		bool get_type (const std::string & key, Type & out) const;

		bool has (const std::string & key) const;

		bool put_string (const std::string & key, const std::string & value);

		bool put_int (const std::string & key, int value);

		bool put_float (const std::string & key, float value);

		bool remove (const std::string & key);

		static bool is_valid_key (const std::string & key);

		static bool is_valid_string_value (const std::string & value);

		const std::map<std::string, std::string> & get_string_map () const;

		const std::map<std::string, int> & get_int_map () const;

		const std::map<std::string, float> & get_float_map () const;

	private:

		friend class Parser;

		std::map<std::string, std::string> string_map;
		std::map<std::string, int> int_map;
		std::map<std::string, float> float_map;

	};

}

