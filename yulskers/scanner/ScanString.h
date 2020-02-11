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

template<char quote, typename CharList, typename String = char_list<>, typename = void>
struct ScanString:
	decltype("Unterminated string literal."_yulskers_error) {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<quote, cs...>, String>
{
	using type = typename MakeStringToken<String>::type;
	using next = Scanner<char_list<cs...>>;
};

template<char quote, char c, char... cs, typename String>
struct ScanString<quote, char_list<c, cs...>, String, std::enable_if_t<c != quote  && c != '\\'>>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, c>> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', '\n', cs...>, String>:
	ScanString<quote, char_list<cs...>, String> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', quote, cs...>, String>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, quote>> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', 'b', cs...>, String>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, '\b'>> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', 'f', cs...>, String>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, '\f'>> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', 'n', cs...>, String>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, '\n'>> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', 'r', cs...>, String>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, '\r'>> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', 't', cs...>, String>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, '\t'>> {};

template<char quote, char... cs, typename String>
struct ScanString<quote, char_list<'\\', 'v', cs...>, String>:
	ScanString<quote, char_list<cs...>, char_list_add_t<String, '\v'>> {};

// TODO escaped u and x

template<char quote, typename String>
struct ScanString<quote, char_list<'\\'>, String>:
	decltype("Invalid escape."_yulskers_error) {};

}
