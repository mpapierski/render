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

	template <typename F>
	inline void operator()(F & res, scope & ctx)
	{
		lhs_(res, ctx);
		rhs_(res, ctx);
	}

	template <typename T>
	add_impl<this_type, typename type_wrapper<T>::type> operator+(T t)
	{
		return add_impl<this_type, typename type_wrapper<T>::type>(*this, t);
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