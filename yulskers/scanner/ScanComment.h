#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

template<bool multiLine, typename Comment>
struct MakeComment;
template<bool multiLine, char... cs>
struct MakeComment<multiLine, char_list<cs...>>
{
	using type = token::Comment<multiLine, cs...>;
};

template<bool multiLine, typename State, typename CharList, typename Comment = char_list<>>
struct ScanComment: Failure<decltype("unterminated comment"_char_list)> {};

template<typename State, char c, char... cs, typename Comment>
struct ScanComment<true, State, char_list<c, cs...>, Comment>:
	ScanComment<true, advance_t<State>, char_list<cs...>, char_list_add_t<Comment, c>> {};

template<typename State, char... cs, typename Comment>
struct ScanComment<true, State, char_list<'*', '/', cs...>, Comment>:
	Scanner<add_token_t<advance_t<State, 2>, typename MakeComment<true, Comment>::type>, char_list<cs...>> {};

template<typename State, char c, char... cs, typename Comment>
struct ScanComment<false, State, char_list<c, cs...>, Comment>:
	ScanComment<false, advance_t<State>, char_list<cs...>, char_list_add_t<Comment, c>> {};

template<typename State, char... cs, typename Comment>
struct ScanComment<false, State, char_list<'\n', cs...>, Comment>:
	Scanner<add_token_t<advance_t<State>, typename MakeComment<false, Comment>::type>, char_list<cs...>> {};

}
