#pragma once

#include <yulskers/scanner/ScannerBase.h>
#include <yulskers/scanner/ScanString.h>
#include <yulskers/scanner/ScanWhitespace.h>
#include <yulskers/scanner/ScanIdentifier.h>
#include <yulskers/scanner/ScanNumber.h>
#include <yulskers/scanner/ScanPlaceholder.h>
#include <yulskers/scanner/ScanComment.h>
#include <yulskers/scanner/ScanHexString.h>

namespace yulskers::scanner {

template<char... cs>
struct Scanner<char_list<'@', cs...>>:
	ScanPlaceholder<char_list<cs...>> {};

template<char c, char... cs>
struct Scanner<char_list<c, cs...>, std::enable_if_t<c == '"' || c == '\''>>:
	ScanString<c, char_list<cs...>> {};

template<char... cs>
struct Scanner<char_list<'h', 'e', 'x', cs...>>:
	ScanHexString<char_list<cs...>> {};

template<char c, char... cs>
struct Scanner<char_list<c, cs...>, std::enable_if_t<isIdentifierStart(c)>>:
	ScanIdentifier<char_list<c, cs...>> {};

template<char c, char... cs>
struct Scanner<char_list<c, cs...>, std::enable_if_t<isDecimalDigit(c)>>:
	ScanNumber<char_list<c, cs...>> {};


template<char c, char... cs>
struct Scanner<char_list<c, cs...>, std::enable_if_t<isWhiteSpace(c)>>:
	ScanWhitespace<char_list<cs...>> {};


template<char... cs>
struct Scanner<char_list<'/', '*', cs...>>:
	ScanComment<true, char_list<cs...>> {};

template<char... cs>
struct Scanner<char_list<'/', '/', cs...>>:
	ScanComment<false, char_list<cs...>> {};

template<char... cs>
struct Scanner<char_list<'{', cs...>>
{
	using type = token::BracketOpen;
	using next = Scanner<char_list<cs...>>;
};

template<char... cs>
struct Scanner<char_list<'}', cs...>>
{
	using type = token::BracketClose;
	using next = Scanner<char_list<cs...>>;
};

template<char... cs>
struct Scanner<char_list<'(', cs...>>
{
	using type = token::ParenOpen;
	using next = Scanner<char_list<cs...>>;
};

template<char... cs>
struct Scanner<char_list<')', cs...>>
{
	using type = token::ParenClose;
	using next = Scanner<char_list<cs...>>;
};

template<char... cs>
struct Scanner<char_list<':', '=', cs...>>
{
	using type = token::Assign;
	using next = Scanner<char_list<cs...>>;
};

template<char... cs>
struct Scanner<char_list<'-', '>', cs...>>
{
	using type = token::Arrow;
	using next = Scanner<char_list<cs...>>;
};

template<char... cs>
struct Scanner<char_list<',', cs...>>
{
	using type = token::Comma;
	using next = Scanner<char_list<cs...>>;
};

template<char... cs>
struct Scanner<char_list<':', cs...>>
{
	using type = token::Comma;
	using next = Scanner<char_list<cs...>>;
};

}
