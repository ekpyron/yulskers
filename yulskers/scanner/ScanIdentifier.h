#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

constexpr bool isDecimalDigit(char c) {	return '0' <= c && c <= '9'; }

constexpr bool isIdentifierStart(char c)
{
	return (c == '_') || (c == '$') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}
constexpr bool isIdentifierPart(char c)
{
	return isIdentifierStart(c) || isDecimalDigit(c) || c == '.';
}

template<typename Identifier>
struct IdentifierOrKeyword;

template<char... cs>
struct IdentifierOrKeyword<char_list<cs...>>
{ using type = token::Identifier<cs...>; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("let"_char_list)>>
{ using type = token::Let; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("if"_char_list)>>
{ using type = token::If; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("switch"_char_list)>>
{ using type = token::Switch; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("case"_char_list)>>
{ using type = token::Case; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("default"_char_list)>>
{ using type = token::Default; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("for"_char_list)>>
{ using type = token::For; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("break"_char_list)>>
{ using type = token::Break; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("continue"_char_list)>>
{ using type = token::Continue; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("function"_char_list)>>
{ using type = token::Function; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("leave"_char_list)>>
{ using type = token::Leave; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("true"_char_list)>>
{ using type = token::True; };

template<>
struct IdentifierOrKeyword<std::decay_t<decltype("false"_char_list)>>
{ using type = token::False; };

template<typename CharList, typename Identifier = char_list<>, typename = void>
struct ScanIdentifier
{
	using type = typename IdentifierOrKeyword<Identifier>::type;
	using next = Scanner<CharList>;
};

template<char c, char... cs, typename Identifier>
struct ScanIdentifier<char_list<c, cs...>, Identifier, std::enable_if_t<isIdentifierPart(c)>>:
	ScanIdentifier<char_list<cs...>, char_list_add_t<Identifier, c>> {};

}
