#include "Map.hpp"
#include "Parser.hpp"

#include <map>

namespace SVDF
{

	template<typename T>
	bool _get_string (const std::string & key, T & out, const std::map<std::string, T> & map)
	{
		auto it = map.find (key);
		if (it != map.end ())
		{
			out = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Map::get_string (const std::string & key, std::string & out) const
	{
		return _get_string (key, out, string_map);
	}

	bool Map::get_int (const std::string & key, int & out) const
	{
		return _get_string (key, out, int_map);
	}

	bool Map::get_float (const std::string & key, float & out) const
	{
		return _get_string (key, out, float_map);
	}

	bool Map::get_type (const std::string & key, Type & out) const
	{
		if (string_map.count (key))
		{
			out = Type::STRING;
		}
		else if (int_map.count (key))
		{
			out = Type::INT;
		}
		else if (float_map.count(key))
		{
			out = Type::FLOAT;
		}
		else
		{
			return false;
		}
		return true;
	}

	bool Map::has (const std::string & key) const
	{
		return string_map.count (key) || int_map.count (key) || float_map.count (key);
	}

	template<typename T, typename O1, typename O2>
	bool _put (const std::string & key, T value, std::map<std::string, T> & map, const std::map<std::string, O1> & other1, const std::map<std::string, O2> & other2)
	{
		if (!other1.count (key) && !other2.count (key))
		{
			return map.emplace (key, value).second;
		}
		else
		{
			return false;
		}
	}

	bool Map::put_string (const std::string & key, const std::string & value)
	{
		return _put (key, value, string_map, int_map, float_map);
	}

	bool Map::put_int (const std::string & key, int value)
	{
		return _put (key, value, int_map, string_map, float_map);
	}

	bool Map::put_float (const std::string & key, float value)
	{
		return _put (key, value, float_map, string_map, int_map);
	}

	bool Map::remove (const std::string & key)
	{
		return string_map.erase (key) || int_map.erase (key) || float_map.erase (key);
	}

	bool Map::is_valid_key (const std::string & key)
	{
		if (key.size () <= Grammar::max_key_length)
		{
			for (char c : key)
			{
				if (!Grammar::is_key_char (c))
				{
					return false;
				}
			}
		}
		return false;
	}

	bool Map::is_valid_string_value (const std::string & value)
	{
		if (value.size () <= Grammar::max_key_length)
		{
			for (char c : value)
			{
				if (!Grammar::is_string_char (c))
				{
					return false;
				}
			}
		}
		return false;
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
}
