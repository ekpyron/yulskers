#pragma once

#include <yulskers/common/type_list.h>
#include <type_traits>
#include <utility>
#include <cstdint>

namespace yulskers {

template<char... cs> struct char_list {};

template<typename T, char c>
struct char_list_add;
template<typename T, char c>
using char_list_add_t = typename char_list_add<T, c>::type;

template<char c, char... cs>
struct char_list_add<char_list<cs...>, c>
{
	using type = char_list<cs..., c>;
};

template<typename Char, Char... c>
constexpr auto operator""_char_list() {	return char_list<c...>{}; }

template<typename Reason>
struct Failure;
template<char... cs>
struct Failure<char_list<cs...>>
{
	using type = Failure<char_list<cs...>>;
	static std::string message() { char msg[sizeof...(cs)] = { cs...}; return std::string(msg, msg + sizeof...(cs)); }
};

template<typename T>
struct is_failure: std::false_type {};
template<typename Reason>
struct is_failure<Failure<Reason>>: std::true_type {};
template<typename T> static constexpr bool is_failure_v = is_failure<std::decay_t<T>>::value;

}
