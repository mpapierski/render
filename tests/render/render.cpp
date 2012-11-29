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

BOOST_AUTO_TEST_CASE (test_lazy_value)
{
	scope s;
	std::string result1 = value(1)(s);
	BOOST_REQUIRE_EQUAL(result1, "1");
	// Evaluate
	std::string result2 = value("Hello world")(s);
	BOOST_REQUIRE_EQUAL(result2, "Hello world");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_type_wrapper)
{
	scope s;
	typedef type_wrapper<int>::type int_;
	std::string result = int_(1234)(s);
	BOOST_REQUIRE_EQUAL(result, "1234");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_each_expression)
{
	scope s;
	list<person> person_list;
	person_list.push_back(person("First name 1", "Last name 1"));
	person_list.push_back(person("First name 2", "Last name 2"));
	// Simple expr. Should output Hello world twice.
	std::string result1 = each(person_list, value("Hello world"))(s);
	BOOST_REQUIRE_EQUAL(result1, "Hello worldHello world");
	// Should output First name 1First name 2
	std::string result2 = each(person_list, get(&person::first_name_))(s);
	BOOST_REQUIRE_EQUAL(result2, "First name 1First name 2");
	// Should output First name 1<br/>First name 2<br/>
	std::string result3 = each(person_list, get(&person::first_name_) + "<br/>")(s);
	BOOST_REQUIRE_EQUAL(result3, "First name 1<br/>First name 2<br/>");
	// Should output <br/>First name 1<br/>First name 2
	std::string result4 = each(person_list, "<br/>" + get(&person::first_name_))(s);
	BOOST_REQUIRE_EQUAL(result4, "<br/>First name 1<br/>First name 2");
	// Should output ->First name 1<-->First name 2<-
	std::string result5 = each(person_list, "->" + get(&person::first_name_) + "<-")(s);
	BOOST_REQUIRE_EQUAL(result5, "->First name 1<-->First name 2<-");
	// HTML list of elements
	std::string result6 = ("<ul>" + each(person_list, "<li>" + get(&person::first_name_) + "</li>") + "</ul>")(s);
	BOOST_REQUIRE_EQUAL(result6, "<ul><li>First name 1</li><li>First name 2</li></ul>");
	
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_each_with_nested_scope)
{
	scope s;
	list<person> person_list_inner;
	list<dummy<0> > dummy_list_outer;
	person_list_inner.push_back(person("INNER", "INNER"));
	dummy_list_outer.push_back(dummy<0>("OUTER"));
	// Simple expr. Should output Hello world twice.
	std::string result = (each(person_list_inner,
		each(dummy_list_outer, get(&person::first_name_) + get(&dummy<0>::id_)))
	)(s);
	BOOST_REQUIRE_EQUAL(result, "INNEROUTER");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_deeply_nested_scope)
{
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
	)(s), std::runtime_error); // dummy<4> is not in this scope!
	BOOST_REQUIRE(s.instances().empty());
	// Valid deeply nested scope. 
	std::string result = each(list0,
		each(list1,
			each(list2,
				each(list3,
					each(list4,
						get(&dummy<0>::id_) // A
						+ get(&dummy<1>::id_) // 1
						+ get(&dummy<2>::id_) // 2
						+ get(&dummy<3>::id_) // 3
						+ get(&dummy<0>::id_) // A
					)
				)
			)
		)
	)(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result.length(), 5);
	BOOST_REQUIRE_EQUAL(result, "A123A");

	std::string result2 = each(list0,
		"start" + get(&dummy<0>::id_) + each(list1,
			each(list2,
				each(list3,
					each(list4,
						get(&dummy<0>::id_) // A
						+ get(&dummy<1>::id_) // 1
						+ get(&dummy<2>::id_) // 2
						+ get(&dummy<3>::id_) // 3
						+ get(&dummy<0>::id_) // A
					)
				)
			)
		) + "stop"
	)(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result2, "start0A123Astop");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_simple_filter_expression_uppercase)
{
	scope s;
	dummy<0> d0("hello world");
	s.push(d0);
	std::string result = (get(&dummy<0>::id_) | uppercase())(s);
	BOOST_REQUIRE_EQUAL(result, "HELLO WORLD");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_simple_filter_expression_lowercase)
{
	scope s;
	dummy<0> d0("HELLO WORLD");
	s.push(d0);
	std::string result = (get(&dummy<0>::id_) | lowercase())(s);
	BOOST_REQUIRE_EQUAL(result, "hello world");
}

//____________________________________________________________________________//
