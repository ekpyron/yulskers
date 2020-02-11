#pragma once

#include <yulskers/common.h>
#include <yulskers/scanner/ScannerState.h>
#include <yulskers/Token.h>

namespace yulskers::scanner {

template<typename CharList, typename = void>
struct Scanner: Failure<decltype("unexpected character"_char_list)> {};

template<>
struct Scanner<char_list<>>
{
	using type = void;
};

}
