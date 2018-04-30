#include "include/Map.hpp"

#include "include/Grammar.hpp"
#include "include/Encodable.hpp"
#include "include/Utils.hpp"

#include <map>
#include <ostream>
#include <string>

namespace SVDF
{

	template<typename T>
	bool _get_string (const std::string & _key, T & _out, const std::map<std::string, T> & _map)
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
	bool _put (const std::string & _key, T _value, std::map<std::string, T> & _map, const std::map<std::string, O1> & _other1, const std::map<std::string, O2> & _other2)
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

	void _assert_valid_key (const std::string & _key)
	{
		if (!Map::is_key_valid (_key))
		{
			throw std::runtime_error ("Invalid key '" + str_utils::user_preview (_key) + "'");
		}
	}

	template <typename T>
	void _assert_valid_key_set (const std::map<std::string, T> & _map)
	{
		for (auto entry : _map)
		{
			_assert_valid_key (entry.first);
		}
	}

	bool Map::get_string (const std::string & _key, std::string & _out) const
	{
		return _get_string (_key, _out, string_map);
	}

	bool Map::get_int (const std::string & _key, int & _out) const
	{
		return _get_string (_key, _out, int_map);
	}

	bool Map::get_float (const std::string & _key, float & _out) const
	{
		return _get_string (_key, _out, float_map);
	}

	bool Map::get_type (const std::string & _key, Type & _out) const
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

	bool Map::has (const std::string & _key) const
	{
		return string_map.count (_key) || int_map.count (_key) || float_map.count (_key);
	}

	bool Map::put_string (const std::string & _key, const std::string & _value)
	{
		return _put (_key, _value, string_map, int_map, float_map);
	}

	bool Map::put_int (const std::string & _key, int _value)
	{
		return _put (_key, _value, int_map, string_map, float_map);
	}

	bool Map::put_float (const std::string & _key, float _value)
	{
		return _put (_key, _value, float_map, string_map, int_map);
	}

	bool Map::remove (const std::string & _key)
	{
		return string_map.erase (_key) || int_map.erase (_key) || float_map.erase (_key);
	}

	const std::map<std::string, std::string>& Map::get_string_map () const
	{
		return string_map;
	}

	const std::map<std::string, int>& Map::get_int_map () const
	{
		return int_map;
	}

	const std::map<std::string, float>& Map::get_float_map () const
	{
		return float_map;
	}

	void Map::encode (std::ostream & _stream, Format _format) const
	{
		_assert_valid_key_set (string_map);
		_assert_valid_key_set (int_map);
		_assert_valid_key_set (float_map);

		bool first = true;

		bool spaces = false;
		bool newlines = false;

		switch (_format)
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
			if (newlines && !first) { _stream << "\n"; }
			first = false;
			_stream << Grammar::string_entry_prefix;
			if (spaces) { _stream << " "; }
			_stream << entry.first;
			if (spaces) { _stream << " "; }
			_stream << Grammar::key_suffix;
			if (spaces) { _stream << " "; }
			_stream << Grammar::string_value_prefix;
			_stream << entry.second;
			_stream << Grammar::string_value_suffix;
			if (spaces && !newlines) { _stream << " "; }
		}
		for (auto entry : int_map)
		{
			if (newlines && !first) { _stream << "\n"; }
			first = false;
			_stream << Grammar::int_entry_prefix;
			if (spaces) { _stream << " "; }
			_stream << entry.first;
			if (spaces) { _stream << " "; }
			_stream << Grammar::key_suffix;
			if (spaces) { _stream << " "; }
			_stream << entry.second;
			if (spaces && !newlines) { _stream << " "; }
		}
		for (auto entry : float_map)
		{
			if (newlines && !first) { _stream << "\n"; }
			first = false;
			_stream << Grammar::float_entry_prefix;
			if (spaces) { _stream << " "; }
			_stream << entry.first;
			if (spaces) { _stream << " "; }
			_stream << Grammar::key_suffix;
			if (spaces) { _stream << " "; }
			_stream << entry.second;
			if (spaces && !newlines) { _stream << " "; }
		}
	}

	bool Map::is_key_valid (const std::string & _key)
	{
		if (_key.size () <= Grammar::max_key_length && !_key.empty ())
		{
			for (char c : _key)
			{
				if (!Grammar::is_key_char (c))
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Map::is_string_valid (const std::string & _string)
	{
		if (_string.size () <= Grammar::max_string_length)
		{
			for (char c : _string)
			{
				if (!Grammar::is_string_char (c))
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

}
