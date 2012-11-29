#if !defined(RENDER_AUX_REFERENCE_WRAPPER_HPP_INCLUDED_)
#define RENDER_AUX_REFERENCE_WRAPPER_HPP_INCLUDED_

namespace render {

/**
 * Wrap reference in a object.
 */
template <typename T>
struct reference_wrapper
{
	T & t_;
	reference_wrapper(T & t)
		: t_(t)
	{
		//
	}

	T & operator*()
	{
		return t_;
	}
};

} /* /namespace render */

#endif /* RENDER_AUX_REFERENCE_WRAPPER_HPP_INCLUDED_ */