#include <type_traits>
#include <functional>
#include <tuple>
#include <variant>
#include <types_list/list.hpp>

using mp::list;
using mp::list_from;

template <typename T, typename U>
static constexpr bool equals = std::is_same<T, U>::value;

static_assert(equals< list<int, int>, list<int, int> >);
static_assert(equals< list<int>, list<int> >);
static_assert(equals< list<>, list<> >);

// list_from
static_assert(equals< list_from<std::tuple<>>, list<> >);
static_assert(equals< list_from<std::tuple<int>>, list<int> >);
static_assert(equals< list_from<std::tuple<char, void>>, list<char, void> >);
static_assert(equals< list_from<std::variant<bool, short>>, list<bool, short> >);

// list::empty
static_assert(list<>::empty);
static_assert(!list<int, double>::empty);

// list::size
static_assert(list<int, char, bool>::size == 3);
static_assert(list<>::push_back<int, int, int>::size == 3);
static_assert(list<int>::push_back<int, int, int>::size == 4);

// list::push_back
static_assert(equals< list<int, char, bool>::push_back<void>, list<int, char, bool, void> >);
static_assert(equals< list<>::push_back<int>, list<int> >);
static_assert(equals< list<>::push_back<void>::push_back<char, short>, list<void, char, short> >);

// list::push_front
static_assert(equals< list<int, char, bool>::push_front<void>, list<void, int, char, bool> >);
static_assert(equals< list<>::push_front<int>, list<int> >);
static_assert(equals< list<>::push_front<void>::push_front<char, short>, list<char, short, void> >);

// list::front
static_assert(equals< list<char>::front, char >);
static_assert(equals< list<int, char, bool>::front, int >);

// list::back
static_assert(equals< list<char>::back, char >);
static_assert(equals< list<int, char, bool>::back, bool >);

// list::get
static_assert(equals< list<char>::get<0>, char >);
static_assert(equals< list<int, char, bool>::get<0>, int >);
static_assert(equals< list<int, char, bool>::get<1>, char >);
static_assert(equals< list<int, char, bool>::get<2>, bool >);

// list::pop_back
static_assert(equals< list<int, char, bool>::pop_back, list<int, char> >);
static_assert(equals< list<int, char>::pop_back, list<int> >);
static_assert(equals< list<int>::pop_back, list<> >);
static_assert(equals< list<int, char, bool>::pop_back::pop_back::pop_back, list<> >);
static_assert(equals< list<>::pop_back, list<> >);

// list::pop_front
static_assert(equals< list<int, char, bool>::pop_front, list<char, bool> >);
static_assert(equals< list<int, char>::pop_front, list<char> >);
static_assert(equals< list<int>::pop_front, list<> >);
static_assert(equals< list<int, char, bool>::pop_front::pop_front::pop_front, list<> >);
static_assert(equals< list<>::pop_front, list<> >);

// list::filter
static_assert(equals< list<>::filter<std::is_array>, list<> >);
static_assert(equals< list<int, double, bool, void>::filter<std::is_integral>, list<int, bool> >);
static_assert(equals< list<int, double>::filter<std::is_arithmetic>::filter<std::is_floating_point>, list<double> >);

// list::reverse
static_assert(equals< list<>::reverse, list<> >);
static_assert(equals< list<int>::reverse, list<int> >);
static_assert(equals< list<int, double, bool, void>::reverse, list<void, bool, double, int> >);

// list::count_if
static_assert(list<>::count_if<std::is_abstract> == 0);
static_assert(list<void>::count_if<std::is_arithmetic> == 0);
static_assert(list<int>::count_if<std::is_arithmetic> == 1);
static_assert(list<int, void, char[], bool, short[2]>::count_if<std::is_array> == 2);

// list::count
static_assert(list<>::count<int> == 0);
static_assert(list<char>::count<int> == 0);
static_assert(list<char>::count<char> == 1);
static_assert(list<int, char, bool>::count<bool> == 1);
static_assert(list<int, char, int, bool, int>::count<int> == 3);

// list::all_of/all_of_v
static_assert(list<>::all_of<std::is_floating_point>::value);
static_assert(list<>::all_of_v<std::is_floating_point>);
static_assert(!list<int, char, float>::all_of<std::is_floating_point>::value);
static_assert(!list<int, char, float>::all_of_v<std::is_floating_point>);
static_assert(list<float, double>::all_of<std::is_floating_point>::value);
static_assert(list<float, double>::all_of_v<std::is_floating_point>);

// list::any_of/any_of_v
static_assert(!list<>::any_of<std::is_floating_point>::value);
static_assert(!list<>::any_of_v<std::is_floating_point>);
static_assert(list<int, char, float>::any_of<std::is_floating_point>::value);
static_assert(list<int, char, float>::any_of_v<std::is_floating_point>);
static_assert(!list<bool, std::size_t>::any_of<std::is_floating_point>::value);
static_assert(!list<bool, std::size_t>::any_of_v<std::is_floating_point>);

// list::none_of/none_of_v
static_assert(list<>::none_of<std::is_floating_point>::value);
static_assert(list<>::none_of_v<std::is_floating_point>);
static_assert(!list<int, char, float>::none_of<std::is_floating_point>::value);
static_assert(!list<int, char, float>::none_of_v<std::is_floating_point>);
static_assert(!list<float, double>::none_of<std::is_floating_point>::value);
static_assert(!list<float, double>::none_of_v<std::is_floating_point>);
static_assert(list<bool, std::size_t>::none_of<std::is_floating_point>::value);
static_assert(list<bool, std::size_t>::none_of_v<std::is_floating_point>);

// list::contains/contains_v
static_assert(!list<>::contains<int>::value);
static_assert(!list<>::contains_v<int>);
static_assert(list<void>::contains<void>::value);
static_assert(list<void>::contains_v<void>);
static_assert(!list<int>::contains<void>::value);
static_assert(!list<int>::contains_v<void>);
static_assert(!list<int, char, bool>::contains<void>::value);
static_assert(!list<int, char, bool>::contains_v<void>);
static_assert(list<int, char, bool>::contains<bool>::value);
static_assert(list<int, char, bool>::contains_v<bool>);

// list::unique
static_assert(equals< list<>::unique, list<> >);
static_assert(equals< list<int>::unique, list<int> >);
static_assert(equals< list<int, int>::unique, list<int> >);
static_assert(equals< list<int, char, char, int, double, double, int, bool, double, void, char, int>::unique, list<int, char, double, bool, void> >);

// list::is_unique/is_unique_v
static_assert(list<int>::is_unique::value);
static_assert(list<int>::is_unique_v);
static_assert(!list<int, int>::is_unique::value);
static_assert(!list<int, int>::is_unique_v);
static_assert(list<int, void, char>::is_unique::value);
static_assert(list<int, void, char>::is_unique_v);

// list::transform
static_assert(equals< list<>::transform<std::decay>, list<> >);
static_assert(equals< list<const int>::transform<std::remove_cv>, list<int> >);
static_assert(equals< list<int[10], int&, const bool&>::transform<std::decay>, list<int*, int, bool> >);

// list::remove
static_assert(equals< list<int>::remove<0>, list<> >);
static_assert(equals< list<int, char, bool>::remove<0>, list<char, bool> >);
static_assert(equals< list<int, char, bool>::remove<1>, list<int, bool> >);
static_assert(equals< list<int, char, bool>::remove<2>, list<int, char> >);

// list::remove_if
static_assert(equals< list<int>::remove_if<std::is_integral>, list<> >);
static_assert(equals< list<std::nullptr_t, char, bool>::remove_if<std::is_null_pointer>, list<char, bool> >);
static_assert(equals< list<double, void, float>::remove_if<std::is_floating_point>, list<void> >);

// list::remove_type
static_assert(equals< list<int>::remove_type<int>, list<> >);
static_assert(equals< list<int, int, int>::remove_type<int>, list<> >);
static_assert(equals< list<int, char, bool>::remove_type<int>, list<char, bool> >);
static_assert(equals< list<int, char, bool>::remove_type<char>, list<int, bool> >);
static_assert(equals< list<int, char, bool>::remove_type<bool>, list<int, char> >);
static_assert(equals< list<int, bool, int, int, void>::remove_type<int>, list<bool, void> >);

// list::unpack_to
static_assert(equals< list<>::unpack_to<std::tuple>, std::tuple<> >);
static_assert(equals< list<int, double>::unpack_to<std::tuple>, std::tuple<int, double> >);
static_assert(equals< list<int, int, double>::unique::unpack_to<std::tuple>, std::tuple<int, double> >);
static_assert(equals< list<float, short>::unpack_to<std::variant>, std::variant<float, short> >);

// list::replace
static_assert(equals< list<int, double>::replace<0, bool>, list<bool, double> >);
static_assert(equals< list<int, double>::replace<1, bool>, list<int, bool> >);
static_assert(equals< list<int, char, double>::replace<2, void>, list<int, char, void> >);

// list::append
static_assert(equals< list<>::append<list<>>, list<> >);
static_assert(equals< list<int>::append<list<>>, list<int> >);
static_assert(equals< list<>::append<list<void>>, list<void> >);
static_assert(equals< list<int>::append<list<double>>, list<int, double> >);
static_assert(equals< list<int>::append<list<char>>::append<list<bool>>, list<int, char, bool> >);

// list::insert
static_assert(equals< list<>::insert<0, int>, list<int> >);
static_assert(equals< list<>::insert<0, char, unsigned char>, list<char, unsigned char> >);
static_assert(equals< list<bool>::insert<0, int>, list<int, bool> >);
static_assert(equals< list<bool>::insert<1, int>, list<bool, int> >);
static_assert(equals< list<char, short>::insert<0, void>, list<void, char, short> >);
static_assert(equals< list<char, short>::insert<1, void>, list<char, void, short> >);
static_assert(equals< list<char, short>::insert<2, void>, list<char, short, void> >);
static_assert(equals< list<char, short>::insert<1, void, double, float>, list<char, void, double, float, short> >);

// list::accumulate
template <auto Value>
using wrap_val = std::integral_constant<decltype(Value), Value>;

static_assert(list<>::accumulate<0> == 0); // std::plus by default
static_assert(list<>::accumulate<10> == 10);
static_assert(list<wrap_val<2>>::accumulate<0> == 2);
static_assert(list<wrap_val<1>, wrap_val<2>, wrap_val<3>>::accumulate<0> == 6);
static_assert(list<wrap_val<1>, wrap_val<2>, wrap_val<3>>::accumulate<10> == 16);
static_assert(list<wrap_val<2>, wrap_val<5>, wrap_val<7>>::accumulate<0, std::minus> == -14);
static_assert(list<wrap_val<2>, wrap_val<5>, wrap_val<7>>::accumulate<1, std::multiplies> == 70);

static_assert(list<wrap_val<false>, wrap_val<false>>::accumulate<false, std::logical_or> == false);
static_assert(list<wrap_val<false>, wrap_val<true>, wrap_val<false>>::accumulate<false, std::logical_or> == true);
static_assert(list<wrap_val<false>, wrap_val<true>, wrap_val<false>>::accumulate<true, std::logical_and> == false);
static_assert(list<wrap_val<true>, wrap_val<true>, wrap_val<true>>::accumulate<true, std::logical_and> == true);

// with types transformation before accumulate
template <typename T>
struct tr
{
	using type = wrap_val<1>;
};

template <>
struct tr<int>
{
	using type = wrap_val<2>;
};

template <>
struct tr<bool>
{
	using type = wrap_val<3>;
};

static_assert(list<char>::transform<tr>::accumulate<0> == 1);
static_assert(list<int>::transform<tr>::accumulate<0> == 2);
static_assert(list<bool>::transform<tr>::accumulate<0> == 3);
static_assert(list<char, int, bool>::transform<tr>::accumulate<0, std::minus> == -6);
static_assert(list<int, int, bool>::transform<tr>::accumulate<1, std::multiplies> == 12);

int main()
{
	return 0;
}
