#define BOOST_TEST_MODULE render
#include <boost/test/included/unit_test.hpp>

#include <render/render.hpp>
#include <sstream>
#include <list>
#include <string>

using namespace std;
using namespace boost;

struct person
{
	string first_name_, last_name_;

	person(string const & first_name, string const & last_name)
		: first_name_(first_name)
		, last_name_(last_name)
	{

	}
};

template <unsigned int N>
struct dummy
{
	string id_;
	dummy(string const & id)
		: id_(id)
	{

	}
};

//____________________________________________________________________________//

struct left_shiftable
{
	const char * val_;
	void operator<<(const char * value)
	{
		val_ = value;
	}
};
	
BOOST_AUTO_TEST_CASE (test_lazy_value)
{
	std::stringstream out1, out2;
	scope s;
	value(1)(out1, s);
	BOOST_REQUIRE_EQUAL(out1.str(), "1");
	left_shiftable temp;
	// Check for type safety in any left-shiftable object.
// Evaluate
	value("Hello world")(temp, s);
	BOOST_REQUIRE_EQUAL(temp.val_, "Hello world");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_type_wrapper)
{
	std::stringstream ss;
	scope s;
	typedef type_wrapper<int>::type int_;
	int_(1234)(ss, s);
	BOOST_REQUIRE_EQUAL(ss.str(), "1234");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_each_expression)
{
	std::stringstream ss1;
	scope s;
	list<person> person_list;
	person_list.push_back(person("First name 1", "Last name 1"));
	person_list.push_back(person("First name 2", "Last name 2"));
	// Simple expr. Should output Hello world twice.
	each(person_list, value("Hello world"))(ss1, s);
	BOOST_REQUIRE_EQUAL(ss1.str(), "Hello worldHello world");
	// Should output First name 1First name 2
	std::stringstream ss2;
	each(person_list, get(&person::first_name_))(ss2, s);
	BOOST_REQUIRE_EQUAL(ss2.str(), "First name 1First name 2");
	// Should output First name 1<br/>First name 2<br/>
	std::stringstream ss3;
	each(person_list, get(&person::first_name_) + "<br/>")(ss3, s);
	BOOST_REQUIRE_EQUAL(ss3.str(), "First name 1<br/>First name 2<br/>");
	// Should output <br/>First name 1<br/>First name 2
	std::stringstream ss4;
	each(person_list, "<br/>" + get(&person::first_name_))(ss4, s);
	BOOST_REQUIRE_EQUAL(ss4.str(), "<br/>First name 1<br/>First name 2");
	// Should output ->First name 1<-->First name 2<-
	std::stringstream ss5;
	each(person_list, "->" + get(&person::first_name_) + "<-")(ss5, s);
	BOOST_REQUIRE_EQUAL(ss5.str(), "->First name 1<-->First name 2<-");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_each_with_nested_scope)
{
	std::stringstream ss1;
	scope s;
	list<person> person_list_inner;
	list<dummy<0> > dummy_list_outer;
	person_list_inner.push_back(person("INNER", "INNER"));
	dummy_list_outer.push_back(dummy<0>("OUTER"));
	// Simple expr. Should output Hello world twice.
	(each(person_list_inner,
		each(dummy_list_outer, get(&person::first_name_) + get(&dummy<0>::id_)))
	)(ss1, s);
	BOOST_REQUIRE_EQUAL(ss1.str(), "INNEROUTER");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_deeply_nested_scope)
{
	std::stringstream ss;
	scope s;
	list<dummy<0> > list0;
	list<dummy<1> > list1;
	list<dummy<2> > list2;
	list<dummy<3> > list3;
	list<dummy<0> > list4; // Same type as list0!
	
	list0.push_back(dummy<0>("0"));
	list1.push_back(dummy<1>("1"));
	list2.push_back(dummy<2>("2"));
	list3.push_back(dummy<3>("3"));
	list4.push_back(dummy<0>("A"));
	// Broken deeply nested scope. 
	BOOST_REQUIRE_THROW(each(list0,
		each(list1,
			each(list2,
				each(list3,
					each(list4,
						get(&dummy<0>::id_)
						+ get(&dummy<1>::id_)
						+ get(&dummy<2>::id_)
						+ get(&dummy<3>::id_)
						+ get(&dummy<4>::id_) // Oops!
					)
				)
			)
		)
	)(ss, s), std::runtime_error); // dummy<4> is not in this scope!
	//BOOST_REQUIRE(s.instances().empty());
	// Valid deeply nested scope. 
	scope s2;
	each(list0,
		each(list1,
			each(list2,
				each(list3,
					each(list4,
						get(&dummy<0>::id_) // 5
						+ get(&dummy<1>::id_) // 2
						+ get(&dummy<2>::id_) // 3
						+ get(&dummy<3>::id_) // 4
						+ get(&dummy<0>::id_) // 5
					)
				)
			)
		)
	)(ss, s2);
	//BOOST_REQUIRE_EQUAL(ss.str().length(), 5);
	BOOST_REQUIRE_EQUAL(ss.str(), "52345");
}

//____________________________________________________________________________//
