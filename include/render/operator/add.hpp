#if !defined(RENDER_OPERATOR_ADD_HPP_INCLUDED_)
#define RENDER_OPERATOR_ADD_HPP_INCLUDED_

template <typename Lhs, typename Rhs>
struct add_impl
{
	typedef add_impl<Lhs, Rhs> this_type;
	Lhs lhs_;
	Rhs rhs_;
	add_impl(Lhs const & lhs, Rhs const & rhs)
		: lhs_(lhs)
		, rhs_(rhs)
	{
		//
	}
	/**
	 * Evaluate.
	 */
	inline std::string operator()(scope & s)
	{
		std::string const & lval = lhs_(s);
		std::string const & rval = rhs_(s);
		return lval + rval;
	}

	template <typename T>
	add_impl<this_type, typename type_wrapper<T>::type> operator+(T t)
	{
		return add_impl<this_type, typename type_wrapper<T>::type>(*this, t);
	}
	
	template <typename F>
	typename F::template impl<this_type>::type operator|(filter_tag<F> const & t)
	{
		return typename F::template impl<this_type>::type(*this);
	}
};

/**
 * Type wrapper specialization.
 * Add will not be wrapped around value_impl.
 */
template <typename Lhs, typename Rhs>
struct type_wrapper<add_impl<Lhs, Rhs> >
{
	typedef add_impl<Lhs, Rhs> type;
};

#endif /* RENDER_OPERATOR_ADD_HPP_INCLUDED_ */