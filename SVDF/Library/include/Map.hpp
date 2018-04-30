#pragma once

#include "Encodable.hpp"

#include <string>
#include <map>
#include <ostream>

namespace SVDF
{

	class Key : public std::string
	{

	public:

		using std::string::string;

		bool is_valid () const;

		static bool is_valid (const std::string & key);

	};

	class String : public std::string
	{

	public:

		using std::string::string;

		bool is_valid () const;

		static bool is_valid (const std::string & string);

	};

	class Parser;

	class Map : public Encodable
	{

	public:

		enum class Type
		{
			STRING, INT, FLOAT
		};

		bool get_string (const Key & key, String & out) const;

		bool get_int (const Key & key, int & out) const;

		bool get_float (const Key & key, float & out) const;

		bool get_type (const Key & key, Type & out) const;

		bool has (const Key & key) const;

		bool put_string (const Key & key, const String & value);

		bool put_int (const Key & key, int value);

		bool put_float (const Key & key, float value);

		bool remove (const Key & key);

		const std::map<Key, String> & get_string_map () const;

		const std::map<Key, int> & get_int_map () const;

		const std::map<Key, float> & get_float_map () const;

		void encode (std::ostream & stream, Format format = Format::NEWLINE) const override;

	private:

		friend class Parser;

		std::map<Key, String> string_map;
		std::map<Key, int> int_map;
		std::map<Key, float> float_map;

	};

}

