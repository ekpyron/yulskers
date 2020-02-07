#pragma once

#include <yulskers/common.h>
#include <yulskers/scanner/ScannerState.h>
#include <yulskers/Token.h>

namespace yulskers::scanner {

template<typename State, typename CharList, typename = void>
struct Scanner: Failure<decltype("unexpected something"_char_list)> {};

template<typename State>
struct Scanner<State, char_list<>>
{
	using type = State;
};

template<typename T, typename = void>
struct scanResultHelper { using type = T; };
template<typename T>
struct scanResultHelper<T, std::void_t<typename T::tokens>> { using type = typename T::tokens; };

template<char... cs>
struct Scan
{
	using scannerResult = typename Scanner<EmptyScannerState, char_list<cs...>>::type;
	using type = typename scanResultHelper<scannerResult>::type;
};

template<char... cs>
using scan = typename Scan<cs...>::type;
}
