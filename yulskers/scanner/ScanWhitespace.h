#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

constexpr inline bool isWhiteSpace(char c) { return c == ' ' || c == '\n' || c == '\t' || c == '\r'; }

template<typename State, typename CharList, typename String = char_list<>, typename = void>
struct ScanWhitespace:
	Scanner<add_token_t<State, token::WhiteSpace>, CharList> {};

template<typename State, char c, char... cs, typename String>
struct ScanWhitespace<State, char_list<c, cs...>, String, std::enable_if_t<isWhiteSpace(c)>>:
	ScanWhitespace<advance_t<State>, char_list<cs...>, char_list_add_t<String, c>> {};

}
