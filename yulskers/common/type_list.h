#pragma once

namespace yulskers {

template<typename...xs>
struct type_list{ static constexpr auto size = sizeof...(xs); };

template<typename Element, typename List>
struct type_list_cons;
template<typename Element, typename List>
using type_list_cons_t = typename type_list_cons<Element, List>::type;
template<typename... Elements, typename Element>
struct type_list_cons<Element, type_list<Elements...>>
{
	using type = type_list<Element, Elements...>;
};

template<typename List, template<typename...> typename Filter, auto value = true>
struct type_list_filter;
template<typename List, template<typename...> typename Filter, auto value = true>
using type_list_filter_t = typename type_list_filter<List, Filter, value>::type;
template<typename Element, typename... Elements, template<typename...> typename Filter, auto value>
struct type_list_filter<type_list<Element, Elements...>, Filter, value>
{
	using type = std::conditional_t<
		Filter<Element>::value == value,
		type_list_cons_t<Element, type_list_filter_t<type_list<Elements...>, Filter, value>>,
		type_list_filter_t<type_list<Elements...>, Filter, value>
	>;
};
template<template<typename...> typename Filter, auto value>
struct type_list_filter<type_list<>, Filter, value>
{
	using type = type_list<>;
};

template<typename List, template<typename...> typename Map>
struct type_list_map;
template<typename List, template<typename...> typename Map>
using type_list_map_t = typename type_list_map<List, Map>::type;
template<typename... Elements, template<typename...> typename Map>
struct type_list_map<type_list<Elements...>, Map>
{
	template<typename... Args>
	using map_t = typename Map<Args...>::type;
	using type = type_list<map_t<Elements>...>;
};

template<typename list1, typename list2>
struct type_list_concat;
template<typename list1, typename list2>
using type_list_concat_t = typename type_list_concat<list1, list2>::type;
template<typename... xs, typename... ys>
struct type_list_concat<type_list<xs...>, type_list<ys...>>
{
	using type = type_list<xs..., ys...>;
};

template<typename list>
struct type_list_reverse;
template<typename list>
using type_list_reverse_t = typename type_list_reverse<list>::type;
template<typename x, typename... xs>
struct type_list_reverse<type_list<x, xs...>>
{
	using type = type_list_concat_t<type_list_reverse_t<type_list<xs...>>, type_list<x>>;
};
template<>
struct type_list_reverse<type_list<>>
{
	using type = type_list<>;
};

template<typename list, std::size_t N = 1, typename = void>
struct type_list_drop_first;
template<typename list, std::size_t N = 1>
using type_list_drop_first_t = typename type_list_drop_first<list, N>::type;
template<typename list>
struct type_list_drop_first<list, 0>
{
	using type = list;
};
template<typename list, std::size_t N>
struct type_list_drop_first<list, N, std::enable_if_t<(N > 1)>>
{
	using type = type_list_drop_first_t<type_list_drop_first_t<list, 1>, N - 1>;
};
template<typename x, typename ...xs>
struct type_list_drop_first<type_list<x, xs...>, 1>
{
	using type = type_list<xs...>;
};

template<typename list>
struct type_list_drop_last { using type = type_list_reverse_t<type_list_drop_first_t<type_list_reverse_t<list>>>; };
template<typename list>
using type_list_drop_last_t = typename type_list_drop_last<list>::type;

template<typename list>
struct type_list_last;
template<typename list>
using type_list_last_t = typename type_list_last<list>::type;
template<typename x, typename... xs>
struct type_list_last<type_list<x, xs...>>:
	type_list_last<type_list<xs...>> {};
template<typename x>
struct type_list_last<type_list<x>>
{ using type = x; };

template<typename list>
struct type_list_first;
template<typename list>
using type_list_first_t = typename type_list_first<list>::type;
template<typename x, typename... xs>
struct type_list_first<type_list<x, xs...>>
{ using type = x; };

template<typename list, std::size_t N, typename = void>
struct type_list_get;
template<typename list, std::size_t N>
using type_list_get_t = typename type_list_get<list, N>::type;

template<typename x, typename... xs, std::size_t N>
struct type_list_get<type_list<x, xs...>, N, std::enable_if_t<(N > 0)>>:
	type_list_get<type_list<xs...>, N-1> {};

template<typename x, typename... xs>
struct type_list_get<type_list<x, xs...>, 0>
{ using type = x; };

}
