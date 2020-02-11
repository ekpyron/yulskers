#pragma once

#include <yulskers/common.h>

namespace yulskers
{

template<typename Reason, typename...>
struct Failure;
template<char... cs, typename... Args>
struct Failure<char_list<cs...>, Args...>
{
	using type = Failure<char_list<cs...>, Args...>;
	static std::string message() { char msg[sizeof...(cs)] = { cs...}; return std::string(msg, msg + sizeof...(cs)); }
};

template<typename T>
struct is_failure: std::false_type {};
template<typename Reason, typename... Args>
struct is_failure<Failure<Reason, Args...>>: std::true_type {};
template<typename T> static constexpr bool is_failure_v = is_failure<std::decay_t<T>>::value;


template<typename result>
struct CheckForFailure
{
	static_assert(!is_failure_v<result>, "");
	using type = result;
};
template<typename result>
using check_for_failure_t = typename CheckForFailure<result>::type;

namespace failure {

template<char...>
struct WhileParsing {};

namespace detail {

template<typename List, typename = void>
struct WhileParsingDropWhitespace;
template<>
struct WhileParsingDropWhitespace<char_list<>> { using type = WhileParsing<>; };
template<char c, char... cs>
struct WhileParsingDropWhitespace<char_list<c, cs...>>
{
	using type = std::conditional_t<
	    isWhiteSpace(c),
		typename WhileParsingDropWhitespace<char_list<cs...>>::type,
		WhileParsing<c, cs...>
	>;
};

}

}

template<typename result>
struct format_reason_if_failure { using type = result; };
template<typename Message, char... cs, typename AST>
struct format_reason_if_failure<Failure<Message, parser::ParserState<scanner::Scanner<char_list<cs...>>, AST>>>
{ using type = Failure<Message, typename failure::detail::WhileParsingDropWhitespace<char_list<cs...>>::type>; };
template<typename result>
using format_reason_if_failure_t = typename format_reason_if_failure<result>::type;

template<typename Char, Char... c>
constexpr auto operator""_yulskers_error() { return Failure<char_list<c...>>{}; }

}
