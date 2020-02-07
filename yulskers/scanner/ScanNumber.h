#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

template<bool isHex, typename Number>
struct MakeNumberToken;
template<char... cs>
struct MakeNumberToken<true, char_list<cs...>>
{
	using type = token::HexNumber<cs...>;
};
template<char... cs>
struct MakeNumberToken<false, char_list<cs...>>
{
	using type = token::DecimalNumber<cs...>;
};

template<typename State, typename CharList, typename Number = char_list<>, typename = void>
struct ScanHexNumber:
	Scanner<add_token_t<State, typename MakeNumberToken<true, Number>::type>, CharList> {};

template<typename State, char c, char... cs, typename Number>
struct ScanHexNumber<State, char_list<c, cs...>, Number, std::enable_if_t<isDecimalDigit(c)>>:
	ScanHexNumber<advance_t<State>, char_list<cs...>, char_list_add_t<Number, c - '0'>> {};

template<typename State, char c, char... cs, typename Number>
struct ScanHexNumber<State, char_list<c, cs...>, Number, std::enable_if_t<'a' <= c && c <= 'f'>>:
	ScanHexNumber<advance_t<State>, char_list<cs...>, char_list_add_t<Number, c - 'a' + 10>> {};

template<typename State, char c, char... cs, typename Number>
struct ScanHexNumber<State, char_list<c, cs...>, Number, std::enable_if_t<'A' <= c && c <= 'F'>>:
	ScanHexNumber<advance_t<State>, char_list<cs...>, char_list_add_t<Number, c - 'A' + 10>> {};

template<typename State, typename CharList, typename Number = char_list<>, typename = void>
struct ScanDecimalNumber:
	Scanner<add_token_t<State,  typename MakeNumberToken<false, Number>::type>, CharList> {};

template<typename State, char c, char... cs, typename Number>
struct ScanDecimalNumber<State, char_list<c, cs...>, Number, std::enable_if_t<isDecimalDigit(c)>>:
	ScanDecimalNumber<advance_t<State>, char_list<cs...>, char_list_add_t<Number, c - '0'>> {};

template<typename State, typename CharList>
struct ScanNumber:
	ScanDecimalNumber<State, CharList> {};

template<typename State, char... cs>
struct ScanNumber<State, char_list<'0', 'x', cs...>>:
	ScanHexNumber<advance_t<State, 2>, char_list<cs...>> {};
}
