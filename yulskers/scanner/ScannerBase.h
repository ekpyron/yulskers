#pragma once

#include <yulskers/common.h>
#include <yulskers/scanner/ScannerState.h>
#include <yulskers/Token.h>

namespace yulskers::scanner {

template<typename CharList, typename>
struct Scanner: decltype("Unexpected character."_yulskers_error) {};

template<>
struct Scanner<char_list<>>
{
	using type = void;
};

}
