#if !defined(RENDER_FILTER_HPP_INCLUDED_)
#define RENDER_FILTER_HPP_INCLUDED_

/**
 * Filter expression tags are wrapped in this "tag".
 */
template <typename T>
struct filter_tag
{

};

#include <render/filter/uppercase.hpp>
#include <render/filter/lowercase.hpp>

#endif /* RENDER_FILTER_HPP_INCLUDED_ */