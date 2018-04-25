#pragma once

#include <vector>
#include <stdexcept>
#include <type_traits>
#include <assert.h>

template <typename Integer, typename Real>
class Pointer
{

	static_assert(std::is_integral<Integer>::value, "Non integer type specified");
	static_assert(std::is_floating_point<Real>::value, "Non floating point type specified");

public:

	virtual bool is_cached () const = 0;

	virtual bool is_real () const = 0;

	virtual int get_component_count () const = 0;

	virtual int get_vector_count () const = 0;

	Real get_real (int index) const
	{
		if (is_real ())
		{
			return provide_real ();
		}
		else
		{
			return provide_integer ();
		}
	}
	
	Integer get_integer (int index) const
	{
		if (is_real ())
		{
			throw std::runtime_error ("Pointer is real");
		}
		else
		{
			return provide_integer ();
		}
	}

	virtual void ensure_cache () = 0;

	virtual ~Pointer () {};

protected:

	virtual Real provide_real (int index) const = 0;

	virtual Integer provide_integer (int index) const = 0;

};

