#pragma once

#include <yulskers/scanner/Scanner.h>
#include <yulskers/parser/Parser.h>

namespace yulskers::literals {

template<typename Char, Char... c>
constexpr auto operator""_yulskers()
{
	return yulskers::parser::parse<c...>{};
}

}