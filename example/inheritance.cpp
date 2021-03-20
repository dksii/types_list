#include <type_traits>
#include <iostream>
#include <types_list/list.hpp>

// inherits from types that passes Check, base classes does not repeat
template <template <typename> class Check>
struct inherit_if
{
private:
	template <typename ...Ts>
	struct inherit : public Ts...
	{};

public:
	template <typename ...Ts>
	using from = typename mp::list<Ts...>::template filter<Check>::unique::template unpack_to<inherit>;
};

// example usage of inherit_if:
struct A
{
	int a;
};

struct B
{
	int b;
};

// checks if T == A or T == B
template <typename T>
using is_a_or_b = mp::list<A, B>::contains<T>;

int main()
{
	inherit_if<is_a_or_b>::from<int, A, void, B, bool, A, B, char> x{}; // x inherits A and B once
	x.a = 1;
	x.b = 2;
	std::cout << "x.a = " << x.a << '\n'; // 1
	std::cout << "x.b = " << x.b << '\n'; // 2

	inherit_if<is_a_or_b>::from<int, A, A, A> y{}; // y inherits A once
	y.a = 3;
	std::cout << "y.a = " << y.a << '\n'; // 3

	inherit_if<is_a_or_b>::from<void, short, unsigned> z{}; // z is empty struct, no inheritance
	std::cout << "sizeof(z) = " << sizeof(z) << '\n'; // 1 (probably)
}