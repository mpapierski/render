#if !defined(RENDER_EXPR_VALUE_HPP_INCLUDED_)
#define RENDER_EXPR_VALUE_HPP_INCLUDED_

#include <render/scope.hpp>

/**
 * Lazy value.
 * Evaluates to the held value.
 */
template <typename T>
struct value_impl
{
	typedef value_impl<T> this_type;
	T repr_;
	inline value_impl(T t)
		: repr_(t)
	{
		//
	}

	/**
	 * Evaluate. Out must be left-shiftable.
	 */
	template <typename Out>
	inline void operator()(Out & out, scope&)
	{
		out << repr_;
	}
};

template <typename T>
value_impl<T> value(T t)
{
	return value_impl<T>(t);
}

#endif /* RENDER_EXPR_VALUE_HPP_INCLUDED_ */