#pragma once

#include <yulskers/scanner/ScannerBase.h>

namespace yulskers::scanner {

template<bool multiLine, typename CharList>
struct ScanComment: decltype("Unterminated comment."_yulskers_error) {};

template<char c, char... cs>
struct ScanComment<true, char_list<c, cs...>>:
	ScanComment<true, char_list<cs...>> {};

template<char... cs>
struct ScanComment<true, char_list<'*', '/', cs...>>:
	Scanner<char_list<cs...>> {};

template<char c, char... cs>
struct ScanComment<false, char_list<c, cs...>>:
	ScanComment<false, char_list<cs...>> {};

template<char... cs>
struct ScanComment<false, char_list<'\n', cs...>>:
	Scanner<char_list<cs...>> {};

}
