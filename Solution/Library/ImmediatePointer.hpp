#pragma once

#include "Pointer.hpp"
#include <vector>

template <typename Integer, typename Real, bool IsReal>
class ImmediatePointer : public Pointer<Integer, Real>
{

public:

	ImmediatePointer (int component_count, int vector_count) 
		: component_count{ component_count }, vector_count{ vector_count } 
	{}

	bool is_cached () const override
	{
		return true;
	}

	bool is_real () const override
	{
		return IsReal;
	}

	int get_component_count () const override
	{
		return component_count;
	}

	int get_vector_count () const override
	{
		return vector_count;
	}

	void ensure_cache () override
	{}

protected:

	Real provide_real (int index) const override
	{
		return Real{};
	}

	Integer provide_integer (int index) const override
	{
		return Integer{};
	}

private:

	const int component_count;
	const int vector_count;

};