#include "Map.hpp"
#include "Grammar.hpp"
#include "Encodable.hpp"

#include <map>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <ostream>
#include <sstream>
#include <string>

namespace SVDF
{

	template<typename T>
	bool _get_string (const Key & _key, T & _out, const std::map<Key, T> & _map)
	{
		auto it = _map.find (_key);
		if (it != _map.end ())
		{
			_out = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename T, typename O1, typename O2>
	bool _put (const Key & _key, T _value, std::map<Key, T> & _map, const std::map<Key, O1> & _other1, const std::map<Key, O2> & _other2)
	{
		if (!_other1.count (_key) && !_other2.count (_key))
		{
			return _map.emplace (_key, _value).second;
		}
		else
		{
			return false;
		}
	}

	void _assert_valid_key (const Key & _key)
	{
		if (!_key.is_valid())
		{
			Key cause_key = _key;
			cause_key.erase (cause_key.begin (), std::find_if (cause_key.begin (), cause_key.end (), std::not1 (std::ptr_fun<int, int> (std::isspace))));
			if (cause_key.size () > 0)
			{
				if (cause_key.size () > 10)
				{
					cause_key.resize (7);
					cause_key.resize (10, '.');
				}
			}
			throw std::runtime_error ("Invalid key '" + _key + "'");
		}
	}

	template <typename T>
	void _assert_valid_key_set (const std::map<Key, T> & _map)
	{
		for (auto entry : _map)
		{
			_assert_valid_key (entry.first);
		}
	}

	bool Map::get_string (const Key & _key, String & _out) const
	{
		return _get_string (_key, _out, string_map);
	}

	bool Map::get_int (const Key & _key, int & _out) const
	{
		return _get_string (_key, _out, int_map);
	}

	bool Map::get_float (const Key & _key, float & _out) const
	{
		return _get_string (_key, _out, float_map);
	}

	bool Map::get_type (const Key & _key, Type & _out) const
	{
		if (string_map.count (_key))
		{
			_out = Type::STRING;
		}
		else if (int_map.count (_key))
		{
			_out = Type::INT;
		}
		else if (float_map.count (_key))
		{
			_out = Type::FLOAT;
		}
		else
		{
			return false;
		}
		return true;
	}

	bool Map::has (const Key & _key) const
	{
		return string_map.count (_key) || int_map.count (_key) || float_map.count (_key);
	}

	bool Map::put_string (const Key & _key, const String & _value)
	{
		return _put (_key, _value, string_map, int_map, float_map);
	}

	bool Map::put_int (const Key & _key, int _value)
	{
		return _put (_key, _value, int_map, string_map, float_map);
	}

	bool Map::put_float (const Key & _key, float _value)
	{
		return _put (_key, _value, float_map, string_map, int_map);
	}

	bool Map::remove (const Key & _key)
	{
		return string_map.erase (_key) || int_map.erase (_key) || float_map.erase (_key);
	}

	const std::map<Key, String>& Map::get_string_map () const
	{
		return string_map;
	}

	const std::map<Key, int>& Map::get_int_map () const
	{
		return int_map;
	}

	const std::map<Key, float>& Map::get_float_map () const
	{
		return float_map;
	}

	void Map::encode (std::ostream & stream, Format format) const
	{
		_assert_valid_key_set (string_map);
		_assert_valid_key_set (int_map);
		_assert_valid_key_set (float_map);

		bool first = true;

		bool spaces = false;
		bool newlines = false;

		switch (format)
		{
			case SVDF::Encodable::Format::NEWLINE_TRUNCATE:
			case SVDF::Encodable::Format::NEWLINE:
				newlines = true;
			case SVDF::Encodable::Format::SPACE:
				spaces = true;
			case SVDF::Encodable::Format::COMPACT:
				break;
		}

		for (auto entry : string_map)
		{
			if (newlines && !first) { stream << "\n"; }
			first = false;
			stream << Grammar::string_entry_prefix;
			if (spaces) { stream << " "; }
			stream << entry.first;
			if (spaces) { stream << " "; }
			stream << Grammar::key_suffix;
			if (spaces) { stream << " "; }
			stream << Grammar::string_value_prefix;
			stream << entry.second;
			stream << Grammar::string_value_suffix;
			if (spaces && !newlines) { stream << " "; }
		}
		for (auto entry : int_map)
		{
			if (newlines && !first) { stream << "\n"; }
			first = false;
			stream << Grammar::int_entry_prefix;
			if (spaces) { stream << " "; }
			stream << entry.first;
			if (spaces) { stream << " "; }
			stream << Grammar::key_suffix;
			if (spaces) { stream << " "; }
			stream << entry.second;
			if (spaces && !newlines) { stream << " "; }
		}
		for (auto entry : float_map)
		{
			if (newlines && !first) { stream << "\n"; }
			first = false;
			stream << Grammar::float_entry_prefix;
			if (spaces) { stream << " "; }
			stream << entry.first;
			if (spaces) { stream << " "; }
			stream << Grammar::key_suffix;
			if (spaces) { stream << " "; }
			stream << entry.second;
			if (spaces && !newlines) { stream << " "; }
		}
	}

	bool Key::is_valid () const
	{
		return is_valid (*this);
	}

	bool Key::is_valid (const std::string & _key)
	{
		if (_key.size () <= Grammar::max_key_length)
		{
			for (char c : _key)
			{
				if (!Grammar::is_key_char (c))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool String::is_valid () const
	{
		return is_valid (*this);
	}

	bool String::is_valid (const std::string & string)
	{
		if (string.size () <= Grammar::max_key_length)
		{
			for (char c : string)
			{
				if (!Grammar::is_string_char (c))
				{
					return false;
				}
			}
		}
		return false;
	}

}
