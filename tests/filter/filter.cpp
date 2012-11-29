#define BOOST_TEST_MODULE render
#include <boost/test/included/unit_test.hpp>

#include <render/render.hpp>
#include <string>

using namespace std;
using namespace boost;

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
