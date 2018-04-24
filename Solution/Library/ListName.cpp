#include "ListName.hpp"

#include <string>
#include <ostream>
#include <istream>

namespace SVDF
{

const std::string ListName::user_defined_name_regexp{ "[A-Za-z_]+" };

const int ListName::no_internal_name{ -1 };
const std::string ListName::no_user_defined_name{ "" };

ListName::ListName (const std::string & _name) : internal_name{ no_internal_name }, user_defined_name{ _name }
{}

ListName::ListName (const int _name) : internal_name{ _name }, user_defined_name{ no_user_defined_name }
{}

bool ListName::is_internal () const
{
	return internal_name != no_internal_name;
}

bool ListName::operator==(const ListName & other) const
{
	return other.internal_name == internal_name
		&& other.user_defined_name == user_defined_name;
}

}
