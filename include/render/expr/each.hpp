#if !defined(RENDER_EXPR_EACH_HPP_INCLUDED_)
#define RENDER_EXPR_EACH_HPP_INCLUDED_

/**
 * Generator class which iterates over a container,
 * using each element as a scope.
 */
template <typename ContainerT, typename Expr>
struct each_impl
{
	typedef each_impl<ContainerT, Expr> this_type;
	/**
	 * Container.
	 */
	ContainerT container_;
	/**
	 * Expression.
	 */
	Expr expr_;
	/**
	 * Construct each expression.
	 */
	each_impl(ContainerT container, Expr expr)
		: container_(container)
		, expr_(expr)
	{

	}
	/**
	 * Evaluate.
	 */
	template <typename T>
	void operator()(T & t, scope & s)
	{
		typedef typename ContainerT::iterator iterator;
		for (iterator it = container_.begin(),
			end = container_.end(); it != end; ++it)
		{
			// Push *it to the scope
			s.push(*it);
			expr_(t, s);
			// Pop from the scope.
			s.pop();
		}
	}
	template <typename T>
	add_impl<this_type, typename type_wrapper<T>::type> operator+(typename type_wrapper<T>::type rhs)
	{
		return add_impl<this_type, typename type_wrapper<T>::type>(*this, rhs);
	}
};

/**
 * Return new lazy "each" generator.
 */
template <typename ContainerT, typename Expr>
each_impl<ContainerT, Expr> each(ContainerT container, Expr expr)
{
	return each_impl<ContainerT, Expr>(container, expr);
}

#endif /* RENDER_EXPR_EACH_HPP_INCLUDED_ */