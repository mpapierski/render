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
	std::string repr_;

	inline value_impl(T const & t)
	{
		std::stringstream ss;
		ss << t;
		repr_ = ss.str();
	}
	/**
	 * Evaluate.
	 */
	inline std::string operator()(scope& s)
	{
		return repr_;
	}
	
	template <typename F>
	typename F::template impl<this_type>::type operator|(filter_tag<F> const & t)
	{
		return typename F::template impl<this_type>::type(*this);
	}
};

template <typename T>
value_impl<T> value(T t)
{
	return value_impl<T>(t);
}

#endif /* RENDER_EXPR_VALUE_HPP_INCLUDED_ */