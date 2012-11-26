#if !defined(RENDER_SCOPE_HPP_INCLUDED_)
#define RENDER_SCOPE_HPP_INCLUDED_

#include <list>
#include <deque>
#include <boost/any.hpp>

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
	 * Push object to a scope.
	 */
	template <typename T>
	void push(T t)
	{
		instances_.push_back(t);
	}
	/**
	 * Pop from a scope.
	 */
	void pop()
	{
		instances_.pop_back();
	}

	template <typename T>
	T get()
	{
		return boost::any_cast<T>(instances_.back());
	}
};

#endif /* RENDER_SCOPE_HPP_INCLUDED_ */
