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

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_lazy_value)
{
	std::stringstream out1, out2;
	scope s;
	value(1)(out1, s);
	BOOST_REQUIRE_EQUAL(out1.str(), "1");
	
	// Check for type safety in any left-shiftable object.
	struct
	{
		const char * val_;
		void operator<<(const char * value)
		{
			val_ = value;
		}
	} temp;
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

BOOST_AUTO_TEST_CASE (test_scope)
{
	person p1("A", "A");
	person p2("B", "B");
	scope s;
	s.push(p1);
	s.push(p2);
	person p = s.get<person>();
	BOOST_REQUIRE_EQUAL(p.first_name_, "B");
	s.pop();
	p = s.get<person>();
	BOOST_REQUIRE_EQUAL(p.first_name_, "A");
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
