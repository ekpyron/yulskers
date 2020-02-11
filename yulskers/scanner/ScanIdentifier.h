#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

constexpr bool isDecimalDigit(char c) {	return '0' <= c && c <= '9'; }
constexpr bool isHexDigit(char c) { return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'); }

constexpr bool isIdentifierStart(char c)
{
	return (c == '_') || (c == '$') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}
constexpr bool isIdentifierPart(char c)
{
	return isIdentifierStart(c) || isDecimalDigit(c) || c == '.';
}
namespace detail {

template<typename Char, Char... c>
constexpr auto operator""_keyword() {	return char_list < c...>{}; }

template<typename Identifier>
struct IdentifierOrKeyword;

template<char... cs>
struct IdentifierOrKeyword<char_list<cs...>>
{ using type = token::Identifier<cs...>; };

template<>
struct IdentifierOrKeyword<decltype("let"_keyword)>
{ using type = token::Let; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("if"_keyword)>>
{ using type = token::If; };

template<>
struct IdentifierOrKeyword<decltype("switch"_keyword)>
{ using type = token::Switch; };

template<>
struct IdentifierOrKeyword<decltype("case"_keyword)>
{ using type = token::Case; };

template<>
struct IdentifierOrKeyword<decltype("default"_keyword)>
{ using type = token::Default; };

template<>
struct IdentifierOrKeyword<decltype("for"_keyword)>
{ using type = token::For; };

template<>
struct IdentifierOrKeyword<decltype("break"_keyword)>
{ using type = token::Break; };

template<>
struct IdentifierOrKeyword<decltype("continue"_keyword)>
{ using type = token::Continue; };

template<>
struct IdentifierOrKeyword<decltype("function"_keyword)>
{ using type = token::Function; };

template<>
struct IdentifierOrKeyword<decltype("leave"_keyword)>
{ using type = token::Leave; };

template<>
struct IdentifierOrKeyword<decltype("true"_keyword)>
{ using type = token::True; };

template<>
struct IdentifierOrKeyword<decltype("false"_keyword)>
{ using type = token::False; };
}

template<typename CharList, typename Identifier = char_list<>, typename = void>
struct ScanIdentifier
{
	using type = typename detail::IdentifierOrKeyword<Identifier>::type;
	using next = Scanner<CharList>;
};

template<char c, char... cs, typename Identifier>
struct ScanIdentifier<char_list<c, cs...>, Identifier, std::enable_if_t<isIdentifierPart(c)>>:
	ScanIdentifier<char_list<cs...>, char_list_add_t<Identifier, c>> {};

}
