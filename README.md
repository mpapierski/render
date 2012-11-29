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
		// Scope
		scope s;
		// Compile
		auto tpl = "<ul>" + each(person_list, "<li>" + get(&person::first_name) + "</li>") + "</ul>";
		std::string result = tpl(s);
		// Output
		std::cout << result << std::endl;
	}

	int main(int argc, char * argv[])
	{
		person_list.emplace("Person 1");
		person_list.emplace("Person 2");
		view_person_list();
	}

This short example will hopefully send `<ul><li>Person 1</li><li>Person 2</li></ul>` to stdout.

## Documentation

### Introduction

#### Scope

A scope simply means the "evaluation scope". Values returned from the evaluated generators are pushed on top of the scope instances stack, and they are removed from the top after generator expression.

	std::list<row> rows;
	each(container, get(&row::id));

There, get(&row::id) knows the current scope which - in this case - each row instance.

	std::list<row> rows;
	std::list<col> cols;
	// …
	each(rows, get(&row::y) + each(cols, get(&col::x)) + "</br>");

Now get(&row::y) knows about its row instances which are present in this scope. Nested generator expression each(cols, …) has no problem with evaluation of &row::y which is in its scope.

Remember that nesting several generator expressions with the containers of the same value_type can be problematic:

	std::list<dummy> list1, list2, list3, list4;
	// …
	each(list1,
		each(list2,
			each(list3,
				each(list4, get(&dummy::value)))))

In this example &dummy::value obtains dummy instance from the top of scope stack - that is elements from list4.

#### Generator

Generator expression produces a content and evaluates specified expression with these values.

See:

* each

#### Filters

Content produced from expression such as `get()`, or `each()` could be filtered. You can use filters this way:

	expression | filter1 | filter2 | filter3 | …

Example:

	each(person_list, get(&person::first_name) + " " + get(&person::last_name)) | uppercase();

Will produce the result of this each expression filtered with uppercase.

The next code is equivalent of the previous. Values of `first_name` and `last_name` are filtered by uppercase filter.

	each(person_list,
		(get(&person::first_name) | uppercase()) + " " +
		(get(&person::last_name) | uppercase()));

### API

#### each(container, expr)

Generator expression.

Iterate over container and for its each value used as a scope evaluate specified expression.

Parameters:

* container: Any standard library container.
* expr: Expression to be evaluated.

#### get(value)

A lazy call to a object instance. When evaluated, uses the *first* valid instance of class from the current evaluation scope as a "this", and "value" as a member. In short:

When value is a pointer-to-member method:

	scope->*value();

When value is a pointer-to-member variable:

	scope->*value;

Parameters:

* value: Pointer-to-member method or variable.

Example:

	scope s;
	query q("the answer to life the universe and everything");
	s.push(q); // q is on top of the stack
	std::string result = get(&query::getResult)(s);
	std::cout << result << std::endl;
	// 42

#### value(value)

Lazy value. Current scope does not matter when evaluated.

Note that the `value` must be convertible to `std::string`.

value(1) evaluates to "1".

#### uppercase()

Makes result uppercase.

#### lowercase()

Makes result lowercase.