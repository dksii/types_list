#pragma once

#include <cstddef>
#include <type_traits>
#include <functional>
#include <tuple>

namespace mp
{
	template <typename T>
	struct type_identity
	{
		using type = T;
	};

	namespace detail
	{
		namespace util
		{
			template <std::size_t Index, typename ...Ts>
			struct get
			{
				static_assert( Index < sizeof...( Ts ), "list::get index out of bounds" );
				using type = typename std::tuple_element<Index, std::tuple<Ts...>>::type;
			};

			template <typename List, typename ...>
			struct pop_back
			{
				using type = List;
			};

			template <typename List, typename T, typename U, typename ...Ts>
			struct pop_back<List, T, U, Ts...>
			{
				using type = typename pop_back<typename List:: template push_back<T>, U, Ts...>::type;
			};

			template <typename List, typename ...Ts>
			struct pop_front
			{
				using type = List;
			};

			template <typename List, typename T, typename ...Ts>
			struct pop_front<List, T, Ts...>
			{
				using type = typename List:: template push_back<Ts...>;
			};

			template<template <typename> class, typename List, typename ...>
			struct filter
			{
				using type = List;
			};

			template<template <typename> class Check, typename List, typename T, typename ...Ts>
			struct filter<Check, List, T, Ts...>
			{
				using type = std::conditional_t<
					Check<T>::value,
					typename filter<Check, typename List::template push_back<T>, Ts...>::type,
					typename filter<Check, List, Ts...>::type
				>;
			};

			template <typename List, typename ...Ts>
			struct reverse
			{
				using type = List;
			};

			template <typename List, typename T, typename ...Ts>
			struct reverse<List, T, Ts...>
			{
				using type = typename reverse<typename List:: template push_front<T>, Ts...>::type;
			};

			template <template <typename> class, std::size_t Count, typename ...Ts>
			struct count_if
			{
				inline static constexpr std::size_t value = Count;
			};

			template <template <typename> class Check, std::size_t Count, typename T, typename ...Ts>
			struct count_if<Check, Count, T, Ts...>
			{
				inline static constexpr std::size_t value = count_if<Check, Check<T>::value ? Count + 1 : Count, Ts...>::value;
			};

			template <typename List, typename ...Ts>
			struct unique
			{
				using type = List;
			};

			template <typename List, typename T, typename ...Ts>
			struct unique<List, T, Ts...>
			{
				using type = std::conditional_t<
					List:: template contains<T>::value,
					typename unique<List, Ts...>::type,
					typename unique<typename List:: template push_back<T>, Ts...>::type
				>;
			};

			template <template <typename> class, typename List, typename ...>
			struct transform
			{
				using type = List;
			};

			template <template <typename> class F, typename List, typename T, typename ...Ts>
			struct transform<F, List, T, Ts...>
			{
				using type = typename transform<F, typename List:: template push_back<typename F<T>::type>, Ts...>::type;
			};

			template <std::size_t Index, typename EmptyList, typename ...Ts>
			struct remove
			{
				static_assert( Index < sizeof...( Ts ), "list::remove index out of bounds" );

			private:
				template <std::size_t, typename List, typename ...>
				struct helper
				{
					using type = List;
				};

				template <std::size_t CurrentIndex, typename List, typename U, typename ...Us>
				struct helper<CurrentIndex, List, U, Us...>
				{
					using type = std::conditional_t<
						CurrentIndex == Index,
						typename List:: template push_back<Us...>,
						typename helper<CurrentIndex + 1, typename List:: template push_back<U>, Us...>::type
					>;
				};

			public:
				using type = typename helper<0, EmptyList, Ts...>::type;
			};

			template <template <typename> class, typename List, typename ...>
			struct remove_if
			{
				using type = List;
			};

			template <template <typename> class Check, typename List, typename T, typename ...Ts>
			struct remove_if<Check, List, T, Ts...>
			{
				using type = std::conditional_t<
					Check<T>::value,
					typename remove_if<Check, List, Ts...>::type,
					typename remove_if<Check, typename List:: template push_back<T>, Ts...>::type
				>;
			};

			template <std::size_t Index, typename NewType, typename EmptyList, typename ...Ts>
			struct replace
			{
				static_assert( Index < sizeof...( Ts ), "list::replace index out of bounds" );

			private:
				template <std::size_t, typename List, typename ...>
				struct helper
				{
					using type = List;
				};

				template <std::size_t CurrentIndex, typename List, typename U, typename ...Us>
				struct helper<CurrentIndex, List, U, Us...>
				{
					using type = std::conditional_t<
						CurrentIndex == Index,
						typename List:: template push_back<NewType, Us...>,
						typename helper<CurrentIndex + 1, typename List:: template push_back<U>, Us...>::type
					>;
				};

			public:
				using type = typename helper<0, EmptyList, Ts...>::type;
			};

			template <typename List, typename>
			struct append;

			template <typename List, template <typename...> class L, typename ...Ts>
			struct append<List, L<Ts...>>
			{
				using type = typename List:: template push_back<Ts...>;
			};

			template <typename EmptyList, std::size_t Index, typename NewList, typename ...Ts>
			struct insert;

			template <typename EmptyList, std::size_t Index, template <typename...> class L, typename ...Us, typename ...Ts>
			struct insert<EmptyList, Index, L<Us...>, Ts...>
			{
				static_assert( Index < sizeof...( Ts ) + 1, "list::insert index out of bounds" );

			private:
				template <std::size_t, typename List, typename ...>
				struct helper
				{
					using type = typename List:: template push_back < Us...>;
				};

				template <std::size_t CurrentIndex, typename List, typename Type, typename ...Types>
				struct helper<CurrentIndex, List, Type, Types...>
				{
					using type = std::conditional_t<
						CurrentIndex == Index,
						typename List:: template push_back<Us..., Type, Types...>,
						typename helper<CurrentIndex + 1, typename List:: template push_back<Type>, Types...>::type
					>;
				};

			public:
				using type = typename helper<0, EmptyList, Ts...>::type;
			};

			template <auto InitialValue, template <typename> class BinaryOp, typename ...Ts>
			struct accumulate
			{
			private:
				using value_type = std::remove_cv_t<decltype( InitialValue )>;
				// must have constexpr ctor and operator()(left, right)
				inline static constexpr BinaryOp<value_type> operation{};

				template <value_type CurrentValue, typename ...Us>
				struct helper
				{
					inline static constexpr value_type value = CurrentValue;
				};

				template <value_type CurrentValue, typename U, typename ...Us>
				struct helper<CurrentValue, U, Us...>
				{
					inline static constexpr value_type value = helper<operation( CurrentValue, U::value ), Us...>::value;
				};

			public:
				inline static constexpr value_type value = helper<InitialValue, Ts...>::value;
			};
		}

		template <typename From, template <typename ...Ts> class What>
		struct create_from;

		template <template <typename ...Ts> class From, template <typename ...Ts> class What, typename ...Ts>
		struct create_from<From<Ts...>, What>
		{
			using type = What<Ts...>;
		};

		template <typename ...Ts>
		struct impl_internal
		{
		private:
			using empty_list = impl_internal<>;

			template <typename T>
			struct check_for
			{
				template <typename U>
				using func = std::is_same<T, U>;
			};

		public:
			inline static constexpr std::size_t size = sizeof...( Ts );
			inline static constexpr bool empty = size == 0;

			template <std::size_t Index>
			using get = typename util::get<Index, Ts...>::type;

			template <typename ...Us>
			using push_back = impl_internal<Ts..., Us...>;

			template <typename ...Us>
			using push_front = impl_internal<Us..., Ts...>;

			using pop_back = typename util::pop_back<empty_list, Ts...>::type;

			using pop_front = typename util::pop_front<empty_list, Ts...>::type;

			template <template <typename> class Check>
			using filter = typename util::filter<Check, empty_list, Ts...>::type;

			using reverse = typename util::reverse<empty_list, Ts...>::type;

			template <template <typename> class Check>
			inline static constexpr std::size_t count_if = util::count_if<Check, 0, Ts...>::value;

			template <typename T>
			inline static constexpr std::size_t count = count_if<check_for<T>:: template func>;

			template <template <typename> class Check>
			using all_of = std::bool_constant<size == count_if<Check>>;

			template <template <typename> class Check>
			inline static constexpr bool all_of_v = all_of<Check>::value;

			template <template <typename> class Check>
			using any_of = std::bool_constant<count_if<Check> != 0>;

			template <template <typename> class Check>
			inline static constexpr bool any_of_v = any_of<Check>::value;

			template <template <typename> class Check>
			using none_of = std::bool_constant<count_if<Check> == 0>;

			template <template <typename> class Check>
			inline static constexpr bool none_of_v = none_of<Check>::value;

			template <typename T>
			using contains = std::bool_constant<count<T> != 0>;

			template <typename T>
			inline static constexpr bool contains_v = contains<T>::value;

			using unique = typename util::unique<empty_list, Ts...>::type;

			using is_unique = std::bool_constant<size == unique::size>;

			inline static constexpr bool is_unique_v = is_unique::value;

			template <template <typename> class F>
			using transform = typename util::transform<F, empty_list, Ts...>::type;

			template <std::size_t Index>
			using remove = typename util::remove<Index, empty_list, Ts...>::type;

			template <template <typename> class Check>
			using remove_if = typename util::remove_if<Check, empty_list, Ts...>::type;

			template <typename T>
			using remove_type = remove_if<check_for<T>:: template func>;

			template <std::size_t Index, typename T>
			using replace = typename util::replace<Index, T, empty_list, Ts...>::type;

			template <typename T>
			using append = typename util::append<impl_internal, T>::type;

			template <template <typename...> class C>
			using unpack_to = C<Ts...>;

			template <std::size_t Pos, typename ...Us>
			using insert = typename util::insert<empty_list, Pos, impl_internal<Us...>, Ts...>::type;

			template <auto InitialValue, template <typename> class BinaryOp = std::plus>
			inline static constexpr decltype( InitialValue ) accumulate = util::accumulate<InitialValue, BinaryOp, Ts...>::value;

			// runtime functions
			template <class F>
			static constexpr F for_each( F func )
			{
				( func( type_identity<Ts>{} ), ... );
				return func;
			}
		};

		template <template <typename...> class TL, typename ...Ts>
		struct list_impl : public impl_internal<Ts...>
		{
		private:
			template <typename List>
			struct unpack;

			template <typename ...Us>
			struct unpack<impl_internal<Us...>>
			{
				using type = TL<Us...>;
			};

			template <typename List>
			using unpack_t = typename unpack<List>::type;

			using impl = impl_internal<Ts...>;

		public:
			template <typename ...Us>
			using push_back = unpack_t<typename impl:: template push_back<Us...>>;

			template <typename ...Us>
			using push_front = unpack_t<typename impl:: template push_front<Us...>>;

			using pop_back = unpack_t<typename impl::pop_back>;

			using pop_front = unpack_t<typename impl::pop_front>;

			template <template <typename> class Check>
			using filter = unpack_t<typename impl:: template filter<Check>>;

			using reverse = unpack_t<typename impl::reverse>;

			using unique = unpack_t<typename impl::unique>;

			template <template <typename> class F>
			using transform = unpack_t<typename impl:: template transform<F>>;

			template <std::size_t Index>
			using remove = unpack_t<typename impl:: template remove<Index>>;

			template <template <typename> class Check>
			using remove_if = unpack_t<typename impl:: template remove_if<Check>>;

			template <typename T>
			using remove_type = unpack_t<typename impl:: template remove_type<T>>;

			template <std::size_t Index, typename T>
			using replace = unpack_t<typename impl:: template replace<Index, T>>;

			template <typename T>
			using append = unpack_t<typename impl:: template append<T>>;

			template <std::size_t Pos, typename ...Us>
			using insert = unpack_t<typename impl:: template insert<Pos, Us...>>;
		};
	}

	template <typename ...Ts>
	struct list : public detail::list_impl<list, Ts...>
	{
		// empty list does not have "front" and "back"
	};

	template <typename T, typename ...Ts>
	struct list<T, Ts...> : public detail::list_impl<list, T, Ts...>
	{
	private:
		template <typename ...Us>
		using TL = list<Us...>; // MSVC fix

		using impl = detail::list_impl<TL, T, Ts...>;

	public:
		using front = typename impl:: template get<0>;
		using back = typename impl:: template get<impl::size - 1>;
	};

	template <typename T>
	using list_from = typename detail::create_from<T, list>::type;
}
