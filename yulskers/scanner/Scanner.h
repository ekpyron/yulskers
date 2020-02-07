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


template<typename State, char... cs>
struct Scanner<State, char_list<'@', cs...>, std::enable_if_t<State::placeholders>>:
	ScanPlaceholder<State, char_list<cs...>> {};


template<typename State, char c, char... cs>
struct Scanner<State, char_list<c, cs...>, std::enable_if_t<c == '"' || c == '\''>>:
	ScanString<c, advance_t<State>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<'h', 'e', 'x', cs...>>:
	ScanHexString<advance_t<State, 3>, char_list<cs...>> {};

template<typename State, char c, char... cs>
struct Scanner<State, char_list<c, cs...>, std::enable_if_t<isIdentifierStart(c)>>:
	ScanIdentifier<State, char_list<c, cs...>> {};

template<typename State, char c, char... cs>
struct Scanner<State, char_list<c, cs...>, std::enable_if_t<isDecimalDigit(c)>>:
	ScanNumber<State, char_list<c, cs...>> {};


template<typename State, char c, char... cs>
struct Scanner<State, char_list<c, cs...>, std::enable_if_t<isWhiteSpace(c)>>:
	ScanWhitespace<advance_t<State>, char_list<cs...>> {};


template<typename State, char... cs>
struct Scanner<State, char_list<'/', '*', cs...>>:
	ScanComment<true, advance_t<State, 2>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<'/', '/', cs...>>:
	ScanComment<false, advance_t<State, 2>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<'{', cs...>>:
	Scanner<add_token_t<advance_t<State>, token::BracketOpen>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<'}', cs...>>:
	Scanner<add_token_t<advance_t<State>, token::BracketClose>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<'(', cs...>>:
	Scanner<add_token_t<advance_t<State>, token::ParenOpen>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<')', cs...>>:
	Scanner<add_token_t<advance_t<State>, token::ParenClose>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<':', '=', cs...>>:
	Scanner<add_token_t<advance_t<State, 2>, token::Assign>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<'-', '>', cs...>>:
	Scanner<add_token_t<advance_t<State, 2>, token::Arrow>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<',', cs...>>:
	Scanner<add_token_t<advance_t<State>, token::Comma>, char_list<cs...>> {};

template<typename State, char... cs>
struct Scanner<State, char_list<':', cs...>>:
	Scanner<add_token_t<advance_t<State>, token::Colon>, char_list<cs...>> {};

}
