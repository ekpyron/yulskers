#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

constexpr bool isHexDigit(char c) { return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'); }

static constexpr std::uint8_t hexDigitsValue(char v)
{
	if ('0' <= v && v <= '9') return v - '0';
	else if ('a' <= v && v <= 'f') return v - 'a' + 10;
	else return v - 'A' + 10;
}

static constexpr std::uint8_t hexDigitsToByte(char a, char b)
{
	return (hexDigitsValue(a) * 16) + hexDigitsValue(b);
}

template<typename T>
struct MakeHexStringToken;
template<char... c>
struct MakeHexStringToken<char_list<c...>>
{
	using type = token::HexString<c...>;
};

template<typename T, char a, char b>
struct addHexByte;
template<typename T, char a, char b>
using addHexByte_t = typename addHexByte<T, a, b>::type;
template<std::uint8_t... bs, char a, char b>
struct addHexByte<token::HexString<bs...>, a, b>
{
	using type = token::HexString<bs..., hexDigitsToByte(a, b)>;
};

template<char quote, typename CharList, typename HexString = token::HexString<>, typename = void>
struct ScanHexStringContent:
	Failure<decltype("invalid or unterminated hex string literal"_char_list)> {};

template<char quote, char c1, char c2, char... cs, typename HexString>
struct ScanHexStringContent<quote, char_list<c1, c2, cs...>, HexString, std::enable_if_t<isHexDigit(c1) && isHexDigit(c2)>>:
	ScanHexStringContent<quote, char_list<cs...>, addHexByte_t<HexString, c1, c2>> {};

template<char quote, char... cs, typename HexString>
struct ScanHexStringContent<quote, char_list<quote, cs...>, HexString>
{
	using type = HexString;
	using next = Scanner<char_list<cs...>>;
};

template<typename CharList, typename = void>
struct ScanHexString:
	Failure<decltype("expected hex string literal"_char_list)> {};

template<char c, char... cs>
struct ScanHexString<char_list<c, cs...>, std::enable_if_t<c == '"' || c == '\''>>:
	ScanHexStringContent<c, char_list<cs...>> {};

}
