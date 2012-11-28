#if !defined(RENDER_EXPR_GET_HPP_INCLUDED_)
#define RENDER_EXPR_GET_HPP_INCLUDED_

#include <stdexcept>

/**
 * Encapsulates pointer-to-member variable as lazy expression.
 */
template <typename Cls, typename T>
struct get_pointer_to_member_variable
{
	typedef get_pointer_to_member_variable<Cls, T> this_type;
	typedef T Cls::*type;
	type t_;
	get_pointer_to_member_variable(type t)
		: t_(t)
	{

	}
	/**
	 * Evaluate
	 */
	std::string operator()(scope & s)
	{
		for (scope::instances_type::const_reverse_iterator it = s.instances().rbegin(),
			end = s.instances().rend(); it != end; ++it)
		{
			try
			{
				reference_wrapper<const Cls> current = boost::any_cast<reference_wrapper<const Cls> >(*it);
				return ((*current).*t_);
			} catch (boost::bad_any_cast)
			{
			}
		}
		throw std::runtime_error("Unable to evaluate get expression with current scope.");
	}

	template <typename F>
	add_impl<this_type, typename type_wrapper<F>::type> operator+(F rhs)
	{
		return add_impl<this_type, typename type_wrapper<F>::type>(*this, rhs);
	}
};

template <typename Cls, typename T>
struct type_wrapper<get_pointer_to_member_variable<Cls, T> >
{
	typedef get_pointer_to_member_variable<Cls, T> type;
};

template <typename F, typename Cls, typename T>
add_impl<typename type_wrapper<F>::type, get_pointer_to_member_variable<Cls, T> >
operator+(F lhs, get_pointer_to_member_variable<Cls, T> rhs)
{
	return add_impl<typename type_wrapper<F>::type, get_pointer_to_member_variable<Cls, T> >(lhs, rhs);
}

template <typename Cls, typename T>
get_pointer_to_member_variable<Cls, T> get(T Cls::*t)
{
	return get_pointer_to_member_variable<Cls, T>(t);
}

#endif /* RENDER_EXPR_GET_HPP_INCLUDED_ */

