#if !defined(RENDER_EXPR_EACH_REFERENCE_HPP_INCLUDED_)
#define RENDER_EXPR_EACH_REFERENCE_HPP_INCLUDED_

namespace render {

/**
 * Generator class which iterates over a container,
 * using each element as a scope.
 */
template <typename ContainerT, typename Expr>
struct reference_each_impl
{
	typedef reference_each_impl<ContainerT, Expr> this_type;
	/**
	 * Container.
	 */
	ContainerT & container_;
	/**
	 * Expression.
	 */
	Expr expr_;
	/**
	 * Construct each expression.
	 */
	reference_each_impl(ContainerT & container, Expr const & expr)
		: container_(container)
		, expr_(expr)
	{

	}
	/**
	 * Evaluate.
	 */
	std::string operator()(scope & s)
	{
		std::string result;
		typedef typename ContainerT::iterator iterator;
		for (iterator it = container_.begin(),
			end = container_.end(); it != end; ++it)
		{
			// Push *it to the scope
			scope_handler handler(s);
			s.push(*it);
			// Evaluate expression with updated scope.
			result += expr_(s);
		}
		return result;
	}
	template <typename T>
	add_impl<this_type, typename type_wrapper<T>::type> operator+(typename type_wrapper<T>::type rhs)
	{
		return add_impl<this_type, typename type_wrapper<T>::type>(*this, rhs);
	}
	
	template <typename F>
	typename F::template impl<this_type>::type operator|(filter_tag<F> const & t)
	{
		return typename F::template impl<this_type>::type(*this);
	}
};

/**
 * Type wrapper specialization.
 * Each expression will not be wrapped around value_impl.
 */
template <typename ContainerT, typename Expr>
struct type_wrapper<reference_each_impl<ContainerT, Expr> >
{
	typedef reference_each_impl<ContainerT, Expr> type;
};

/**
 * Concatenate "any" type with each expr.
 */
template <typename T, typename ContainerT, typename Expr>
add_impl<typename type_wrapper<T>::type, reference_each_impl<ContainerT, Expr> > operator+(T t, reference_each_impl<ContainerT, Expr> const & expr)
{
	return add_impl<typename type_wrapper<T>::type, reference_each_impl<ContainerT, Expr> >(t, expr);
}

/**
 * Return new lazy "each" generator.
 */
template <typename ContainerT, typename Expr>
reference_each_impl<ContainerT, Expr> each(ContainerT & container, Expr expr)
{
	return reference_each_impl<ContainerT, Expr>(container, expr);
}

} /* /namespace render */

#endif /* RENDER_EXPR_EACH_REFERENCE_HPP_INCLUDED_ */