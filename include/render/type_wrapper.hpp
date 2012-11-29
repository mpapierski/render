#if !defined(RENDER_EXPR_WRAPPER_HPP_INCLUDED_)
#define RENDER_EXPR_WRAPPER_HPP_INCLUDED_

namespace render {

/**
 * A type wrapper which knows about existing expressions,
 * and encapsulates every other as a "lazy value".
 */
template <typename T>
struct type_wrapper
{
	typedef value_impl<T> type;
};

} /* /namespace render */

#endif /* RENDER_EXPR_WRAPPER_HPP_INCLUDED_ */