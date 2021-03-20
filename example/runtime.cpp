#include <cstddef>
#include <string>
#include <typeinfo>
#include <iostream>
#include <unordered_map>
#include <types_list/list.hpp>

struct PrinterObj
{
	template <typename T>
	void operator()(mp::type_identity<T>)
	{
		std::cout << typeid(T).name() << '\n';
	}
};

struct CountMap
{
	template <typename T>
	void operator()(mp::type_identity<T>)
	{
		map[typeid(T).name()]++;
	}

	std::unordered_map<std::string, std::size_t> map;
};

using l = mp::list<int, bool, char>;
using l2 = mp::list<void, void, int, bool, bool, char, bool>;

int main()
{
	// print list
	l::for_each(PrinterObj{}); // via functional object

	l::for_each([] (auto t) {  // via lambda
		std::cout << typeid(typename decltype(t)::type).name() << '\n';
	});

	// build map: type name -> count
	auto [map] = l2::for_each(CountMap{});

	for (const auto& [k, v] : map)
	{
		std::cout << k << ": " << v << '\n';
	}
}