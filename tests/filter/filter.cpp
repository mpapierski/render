#define BOOST_TEST_MODULE render
#include <boost/test/included/unit_test.hpp>

#include <render/render.hpp>
#include <string>

using namespace std;
using namespace boost;
using namespace render;

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

BOOST_AUTO_TEST_CASE (test_each_expression_as_uppercase)
{
	scope s;
	std::list<dummy<0> > d;
	d.push_back(dummy<0>("h"));
	d.push_back(dummy<0>("e"));
	d.push_back(dummy<0>("l"));
	d.push_back(dummy<0>("l"));
	d.push_back(dummy<0>("o"));
	d.push_back(dummy<0>(" "));
	d.push_back(dummy<0>("w"));
	d.push_back(dummy<0>("o"));
	d.push_back(dummy<0>("r"));
	d.push_back(dummy<0>("l"));
	d.push_back(dummy<0>("d"));
	// value_impl is filtered
	std::string result0 = (value("hello world") | uppercase())(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result0, "HELLO WORLD");
	// each_impl is filtered
	std::string result1 = (each(d, get(&dummy<0>::id_)) | uppercase())(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result1, "HELLO WORLD");
	// add_impl is filtered
	std::string result2 = (("begin " + each(d, get(&dummy<0>::id_)) + " end") | uppercase())(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result2, "BEGIN HELLO WORLD END");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_each_expression_as_lowercase)
{
	scope s;
	std::list<dummy<0> > d;
	d.push_back(dummy<0>("H"));
	d.push_back(dummy<0>("E"));
	d.push_back(dummy<0>("L"));
	d.push_back(dummy<0>("L"));
	d.push_back(dummy<0>("O"));
	d.push_back(dummy<0>(" "));
	d.push_back(dummy<0>("W"));
	d.push_back(dummy<0>("O"));
	d.push_back(dummy<0>("R"));
	d.push_back(dummy<0>("L"));
	d.push_back(dummy<0>("D"));
	// value_impl is filtered
	std::string result0 = (value("HELLO WORLD") | lowercase())(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result0, "hello world");
	// each_impl is filtered
	std::string result1 = (each(d, get(&dummy<0>::id_)) | lowercase())(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result1, "hello world");
	// add_impl is filtered
	std::string result2 = (("BEGIN " + each(d, get(&dummy<0>::id_)) + " END") | lowercase())(s);
	BOOST_REQUIRE(s.instances().empty());
	BOOST_REQUIRE_EQUAL(result2, "begin hello world end");
}


//____________________________________________________________________________//

