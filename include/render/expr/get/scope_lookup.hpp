#if !defined(RENDER_EXPR_GET_SCOPE_LOOKUP_HPP_INCLUDED_)
#define RENDER_EXPR_GET_SCOPE_LOOKUP_HPP_INCLUDED_

namespace render {

/**
 * Looks up for the most recent T class instance
 * in the evaluation scope.
 */
template <typename T>
struct scope_lookup_impl
{
	scope_lookup_impl()
	{
	}

	T & operator()(scope & s)
	{
		for (scope::instances_type::const_reverse_iterator it = s.instances().rbegin(),
			end = s.instances().rend(); it != end; ++it)
		{
			try
			{
				reference_wrapper<T> current = boost::any_cast<reference_wrapper<T> >(*it);
				return *current;
			} catch (boost::bad_any_cast)
			{
			}
		}
		throw std::runtime_error("Unable to evaluate get expression with current scope.");
	}
};

template <typename T>
scope_lookup_impl<T> get()
{
	return scope_lookup_impl<T>();
}

} /* /namespace render */

#endif /* RENDER_EXPR_GET_SCOPE_LOOKUP_HPP_INCLUDED_ */
