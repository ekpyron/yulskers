#pragma once

#include <type_traits>

namespace yulskers {

template<typename...xs>
struct type_list{ static constexpr auto size = sizeof...(xs); };

template<typename Element, typename List>
struct type_list_cons;
template<typename Element, typename List>
using type_list_cons_t = typename type_list_cons<Element, List>::type;
template<typename... Elements, typename Element>
struct type_list_cons<Element, type_list<Elements...>>
{ using type = type_list<Element, Elements...>; };

template<typename list1, typename list2>
struct type_list_concat;
template<typename list1, typename list2>
using type_list_concat_t = typename type_list_concat<list1, list2>::type;
template<typename... xs, typename... ys>
struct type_list_concat<type_list<xs...>, type_list<ys...>>
{ using type = type_list<xs..., ys...>; };

template<typename list>
struct type_list_drop_first;
template<typename x, typename... xs>
struct type_list_drop_first<type_list<x, xs...>>
{ using type = type_list<xs...>; };
template<typename list, std::size_t N = 1>
using type_list_drop_first_t = typename type_list_drop_first<list>::type;

template<typename list>
struct type_list_first;
template<typename list>
using type_list_first_t = typename type_list_first<list>::type;
template<typename x, typename... xs>
struct type_list_first<type_list<x, xs...>>
{ using type = x; };

}
