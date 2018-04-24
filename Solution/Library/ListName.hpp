#pragma once

#include <string>
#include <ostream>
#include <istream>

namespace SVDF
{

class ListName
{

public:

	static const std::string user_defined_name_regexp;

	static const int no_internal_name;
	static const std::string no_user_defined_name;

	const int internal_name;
	const std::string user_defined_name;

	ListName (const std::string& user_defined_name);

	ListName (const int internal_name);

	bool is_internal () const;

	bool operator == (const ListName & other) const;

};

}
