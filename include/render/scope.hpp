#if !defined(RENDER_SCOPE_HPP_INCLUDED_)
#define RENDER_SCOPE_HPP_INCLUDED_

#include <list>
#include <deque>
#include <boost/any.hpp>

namespace render {

/**
 * Evaluation scope.
 */
struct scope
{
	typedef std::deque<boost::any> instances_type;
	/**
	 * Instances in this scope.
	 */
	instances_type instances_;
	/**
	 * Get instances list.
	 */
	instances_type & instances()
	{
		return instances_;
	}
	/**
	 * Push object to a scope.
	 */
	template <typename T>
	void push(T & t)
	{
		instances_.push_back(reference_wrapper<T>(t));
	}
	/**
	 * Pop from a scope.
	 */
	void pop()
	{
		instances_.pop_back();
	}
};

/**
 * RAII-enabled scope handler.
 */
struct scope_handler
{
	scope & scope_;
	scope_handler(scope & s)
		: scope_(s)
	{
	}
	~scope_handler()
	{
		if (!scope_.instances().empty())
		{
			scope_.pop();
		}	
	}
};

} /* /namespace render */

#endif /* RENDER_SCOPE_HPP_INCLUDED_ */
