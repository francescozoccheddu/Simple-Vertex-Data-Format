#pragma once

#include "Pointer.hpp"

template <typename Integer, typename Real>
class CachedPointer : Pointer<Integer, Real>
{

public:

	virtual bool is_cached () const
	{
		return cache != nullptr;
	}

	virtual int get_component_count () const = 0;

	virtual int get_vector_count () const = 0;

	Real get_real (int index)
	{

	}

	Integer get_integer (int index);

	void ensure_cache ()
	{
		if (!is_cached ())
		{
			cache = new ImmediateBuffer (get_component_count (), get_vector_count ());
		}
	}

	virtual ~Pointer ()
	{
		if (cache != nullptr)
		{
			delete cache;
			cache = nullptr;
		}
	}

protected:

	virtual Real provide_real (int index) const = 0;

	virtual Integer provide_integer (int index) const = 0;

	virtual Real compute_real (int index) const = 0;

	virtual Integer compute_integer (int index) const = 0;

private:

	ImmediatePointer * cache{ nullptr };

};