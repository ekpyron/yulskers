#pragma once

#include <yulskers/scanner/Scanner.h>
#include <yulskers/parser/Parser.h>

namespace yulskers::literals {

template<typename Char, Char... c>
constexpr auto operator""_yulskers()
{
	using tokens = yulskers::scanner::scan<c...>;
	if constexpr (yulskers::is_failure_v<tokens>) {
		return tokens {};
	} else {
		return yulskers::parser::parse<tokens>{};
	}
}

}