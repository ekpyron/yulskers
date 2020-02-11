#pragma once

namespace yulskers {

template<char... cs> struct char_list {};

template<typename T, char c>
struct char_list_add;
template<typename T, char c>
using char_list_add_t = typename char_list_add<T, c>::type;

template<char c, char... cs>
struct char_list_add<char_list<cs...>, c>
{ using type = char_list<cs..., c>; };

constexpr inline bool isWhiteSpace(char c) { return c == ' ' || c == '\n' || c == '\t' || c == '\r'; }

}
