#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

template<typename String>
struct MakeStringToken;
template<char... cs>
struct MakeStringToken<char_list<cs...>>
{
	using type = token::String<cs...>;
};

// TODO: this is rather solidity strings than yul strings

template<char quote, typename State, typename CharList, typename String = char_list<>, typename = void>
struct ScanString:
	Failure<decltype("unterminated string literal"_char_list)> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<quote, cs...>, String>:
	Scanner<add_token_t<advance_t<State>, typename MakeStringToken<String>::type>, char_list<cs...>> {};

template<char quote, typename State, char c, char... cs, typename String>
struct ScanString<quote, State, char_list<c, cs...>, String, std::enable_if_t<c != quote  && c != '\\'>>:
	ScanString<quote, advance_t<State>, char_list<cs...>, char_list_add_t<String, c>> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', '\n', cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, String> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', quote, cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, char_list_add_t<String, quote>> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', 'b', cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, char_list_add_t<String, '\b'>> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', 'f', cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, char_list_add_t<String, '\f'>> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', 'n', cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, char_list_add_t<String, '\n'>> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', 'r', cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, char_list_add_t<String, '\r'>> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', 't', cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, char_list_add_t<String, '\t'>> {};

template<char quote, typename State, char... cs, typename String>
struct ScanString<quote, State, char_list<'\\', 'v', cs...>, String>:
	ScanString<quote, advance_t<State, 2>, char_list<cs...>, char_list_add_t<String, '\v'>> {};

// TODO escaped u and x

template<char quote, typename State, typename String>
struct ScanString<quote, State, char_list<'\\'>, String>:
	Failure<decltype("invalid escape"_char_list)> {};

}
