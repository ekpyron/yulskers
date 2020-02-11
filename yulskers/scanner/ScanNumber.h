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

template<typename CharList, typename Number = char_list<>, typename = void>
struct ScanHexNumber
{
	using type = typename MakeNumberToken<true, Number>::type;
	using next = Scanner<CharList>;
};

template<char c, char... cs, typename Number>
struct ScanHexNumber<char_list<c, cs...>, Number, std::enable_if_t<isDecimalDigit(c)>>:
	ScanHexNumber<char_list<cs...>, char_list_add_t<Number, c - '0'>> {};

template<char c, char... cs, typename Number>
struct ScanHexNumber<char_list<c, cs...>, Number, std::enable_if_t<'a' <= c && c <= 'f'>>:
	ScanHexNumber<char_list<cs...>, char_list_add_t<Number, c - 'a' + 10>> {};

template<char c, char... cs, typename Number>
struct ScanHexNumber<char_list<c, cs...>, Number, std::enable_if_t<'A' <= c && c <= 'F'>>:
	ScanHexNumber<char_list<cs...>, char_list_add_t<Number, c - 'A' + 10>> {};

template<typename CharList, typename Number = char_list<>, typename = void>
struct ScanDecimalNumber
{
	using type = typename MakeNumberToken<false, Number>::type;
	using next = Scanner<CharList>;
};

template<char c, char... cs, typename Number>
struct ScanDecimalNumber<char_list<c, cs...>, Number, std::enable_if_t<isDecimalDigit(c)>>:
	ScanDecimalNumber<char_list<cs...>, char_list_add_t<Number, c - '0'>> {};

template<typename CharList>
struct ScanNumber:
	ScanDecimalNumber<CharList> {};

template<char... cs>
struct ScanNumber<char_list<'0', 'x', cs...>>:
	ScanHexNumber<char_list<cs...>> {};
}
