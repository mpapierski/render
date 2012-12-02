#if !defined(RENDER_EXPR_EACH_SCOPED_HPP_INCLUDED_)
#define RENDER_EXPR_EACH_SCOPED_HPP_INCLUDED_

namespace render {

/**
 * Looks up for container instance in a evaluation scope.
 */
template <typename T, typename Expr>
struct scoped_each_impl
{
	scope_lookup_impl<T> lookup_;
	Expr expr_;
	scoped_each_impl(scope_lookup_impl<T> lookup, Expr const & expr)
		: lookup_(lookup)
		, expr_(expr)
	{
	}

	std::string operator()(scope & s)
	{
		T & instance = lookup_(s);
		return reference_each_impl<T, Expr>(instance, expr_)(s);
	}
};

template <typename T, typename Expr>
scoped_each_impl<T, Expr> each(scope_lookup_impl<T> lookup, Expr expr)
{
	return scoped_each_impl<T, Expr>(lookup, expr);
}

} /* /namespace render */

#endif /* RENDER_EXPR_EACH_SCOPED_HPP_INCLUDED_ */