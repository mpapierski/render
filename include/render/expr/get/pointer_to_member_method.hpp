#if !defined(RENDER_EXPR_GET_POINTER_TO_MEMBER_METHOD_HPP_INCLUDED_)
#define RENDER_EXPR_GET_POINTER_TO_MEMBER_METHOD_HPP_INCLUDED_

#include <sstream>
#include <stdexcept>

namespace render {

/**
 * Encapsulates pointer-to-member variable as lazy expression.
 */
template <typename Cls, typename T>
struct get_pointer_to_member_method: scope_lookup_impl<Cls>
{
	typedef get_pointer_to_member_method<Cls, T> this_type;
	typedef T (Cls::*type)();
	type t_;
	get_pointer_to_member_method(type t)
		: t_(t)
	{
	}

	/**
	 * Evaluate
	 */
	std::string operator()(scope & s)
	{
		Cls & instance = scope_lookup_impl<Cls>::operator()(s);
		std::stringstream ss;
		ss << (instance.*t_)();
		return ss.str();
	}

	template <typename F>
	add_impl<this_type, typename type_wrapper<F>::type> operator+(F rhs)
	{
		return add_impl<this_type, typename type_wrapper<F>::type>(*this, rhs);
	}

	template <typename F>
	typename F::template impl<this_type>::type operator|(filter_tag<F> const & t)
	{
		return typename F::template impl<this_type>::type(*this);
	}
};

template <typename Cls, typename T>
struct type_wrapper<get_pointer_to_member_method<Cls, T> >
{
	typedef get_pointer_to_member_method<Cls, T> type;
};

template <typename F, typename Cls, typename T>
add_impl<typename type_wrapper<F>::type, get_pointer_to_member_method<Cls, T> >
operator+(F lhs, get_pointer_to_member_method<Cls, T> rhs)
{
	return add_impl<typename type_wrapper<F>::type, get_pointer_to_member_method<Cls, T> >(lhs, rhs);
}

template <typename Cls, typename T>
get_pointer_to_member_method<Cls, T> get(T (Cls::*t)())
{
	return get_pointer_to_member_method<Cls, T>(t);
}

} /* /namespace render */

#endif /* RENDER_EXPR_GET_POINTER_TO_MEMBER_METHOD_HPP_INCLUDED_ */

