#if !defined(RENDER_FILTER_LOWERCASE_HPP_INCLUDED_)
#define RENDER_FILTER_LOWERCASE_HPP_INCLUDED_

#include <algorithm>
#include <functional>
#include <locale>

/**
 * Implementation of "lowercase filter".
 */
template <typename Lhs>
struct lowercase_impl
{
	Lhs lhs_;
	lowercase_impl(Lhs const & lhs)
		: lhs_(lhs)
	{
	}
	/**
	 * Evaluate LHS expression, and lowercase it.
	 */
	std::string operator()(scope & s)
	{
		std::locale loc;
		std::string eval = lhs_(s);
		std::transform(eval.begin(), eval.end(), eval.begin(),
			std::bind2nd(std::ptr_fun(&std::tolower<char>), loc));
		return eval;
	}
};

/**
 * Uppercase tag.
 * We can pass it as a non-templated type,
 * and then from the other expressions we can get
 * straight to its implementation using a templated "rebind".
 */
struct lowercase_tag
{
	template <typename T>
	struct impl
	{
		typedef lowercase_impl<T> type;
	};
};

filter_tag<lowercase_tag> lowercase()
{
	return filter_tag<lowercase_tag>();
}

#endif /* RENDER_FILTER_LOWERCASE_HPP_INCLUDED_ */