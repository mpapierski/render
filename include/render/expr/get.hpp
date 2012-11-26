#if !defined(RENDER_EXPR_GET_HPP_INCLUDED_)
#define RENDER_EXPR_GET_HPP_INCLUDED_

/**
 * Encapsulates pointer-to-member variable as lazy expression.
 */
template <typename Cls, typename T>
struct get_pointer_to_member_variable
{
	typedef T Cls::*type;
	type t_;
	get_pointer_to_member_variable(type t)
		: t_(t)
	{

	}

	template <typename F>
	void operator()(F & t, scope & s)
	{
		Cls current = s.get<Cls>();
		t << (current.*t_);
	}
};

template <typename Cls, typename T>
get_pointer_to_member_variable<Cls, T> get(T Cls::*t)
{
	return get_pointer_to_member_variable<Cls, T>(t);
}

#endif /* RENDER_EXPR_GET_HPP_INCLUDED_ */

