# render [![Build Status](https://secure.travis-ci.org/mpapierski/render.png)](http://travis-ci.org/mpapierski/render)

Type safe compile-time templates for C++.

## Introduction

With render you can produce dynamic content in an intuitive domain-specific language. Focused to be helpful in areas such as web development.

## Features

* No external dependencies. Relying on standard library.
* Compatible with any standard library container.
* Compatible with any class or structure with no boilerplate code.
* Extensive test suite.
* Almost no overhead over "traditional" approach.
* Compatible with C++03.

## Example

Please note that an actual API is a subject of changes.

	struct person
	{
		std::string name_;
		person(std::string const & name)
			: name_(name)
		{

		}

		std::string first_name()
		{
			return name_;
		}
	};

	static std::list<person> person_list;

	void view_person_list()
	{
		// Compile
		auto tpl = "<ul>" + each(person_list, "<li>" + get(&person::first_name) + "</li>") + "</ul>";
		// Render
		std::stringstream ss;
		tpl(ss);
		// Output
		std::cout << ss.str() << std::endl;
	}

	int main(int argc, char * argv[])
	{
		person_list.emplace("Person 1");
		person_list.emplace("Person 2");
		view_person_list();
	}

This short example will hopefully send "<ul><li>Person 1</li><li>Person 2</li></ul>" to stdout.

## Documentation

### Introduction

#### Scope

A scope means the current "evaluation scope". Expression evaluted by the generators are evaluted with the current value returned from it.

	std::list<row> rows;
	each(container, get(&row::id));

There, get(&row::id) knows the current scope which - in this case - each row instance.

	std::list<row> rows;
	std::list<cols> cols;
	each(rows, get(&row::y) + each(cols, get(&col::x)) + "</br>");

Now get(&row::y) knows about its row instances which are present in this scope. Nested generator expression each(cols, …) does not know about its inner scope therefore it will not compile.

#### Generator

Generator expression produces a content and evaluates specified expression with these values.

See:

* each

### API

#### each(container, expr)

Generator expression.

Iterate over container and for its each value used as a scope evaluate specified expression.

Parameters:

* container: Any standard library container.
* expr: Expression to be evaluated.

#### get(value)

A lazy call to a object instance. When evaluated, uses the current scope as a "this", and "value" as a member. In short:

When value is a pointer-to-member method:

	scope->*value();

When value is a pointer-to-member variable:

	scope->*value;

Parameters:

* value: Pointer-to-member method or variable.

	query q("the answer to life the universe and everything");
	std::stringstream ss;
	get(&q::getResult)(ss, q);
	std::cout << ss.str() << std::endl;
	// 42

#### value(value)

Lazy value. Current scope does not matter.

value(1) evaluates to "1".