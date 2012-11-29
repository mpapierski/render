#if !defined(RENDER_FILTER_UPPERCASE_HPP_INCLUDED_)
#define RENDER_FILTER_UPPERCASE_HPP_INCLUDED_

#include <algorithm>
#include <functional>
#include <locale>

namespace render {

/**
 * Implementation of "uppercase filter".
 */
template <typename Lhs>
struct uppercase_impl
{
	Lhs lhs_;
	uppercase_impl(Lhs const & lhs)
		: lhs_(lhs)
	{
	}
	/**
	 * Evaluate LHS expression, and uppercase it.
	 */
	std::string operator()(scope & s)
	{
		std::locale loc;
		std::string eval = lhs_(s);
		std::transform(eval.begin(), eval.end(), eval.begin(),
			std::bind2nd(std::ptr_fun(&std::toupper<char>), loc));
		return eval;
	}
};

/**
 * Uppercase tag.
 * We can pass it as a non-templated type,
 * and then from the other expressions we can get
 * straight to its implementation using a templated "rebind".
 */
struct uppercase_tag
{
	template <typename T>
	struct impl
	{
		typedef uppercase_impl<T> type;
	};
};

filter_tag<uppercase_tag> uppercase()
{
	return filter_tag<uppercase_tag>();
}

} /* /namespace render */

#endif /* RENDER_FILTER_UPPERCASE_HPP_INCLUDED_ */