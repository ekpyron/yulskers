#pragma once

#include <yulskers/scanner/Scanner.h>
#include <yulskers/parser/Parser.h>

namespace yulskers::literals {

template<typename Char, Char... c>
constexpr auto operator""_yulskers()
{
	using result = yulskers::parser::parse<yulskers::parser::ParseBlock, c...>;
	return check_for_failure_t<result>{};
}

template<typename Char, Char... c>
constexpr auto operator""_yulskers_block()
{
	using result = yulskers::parser::parse<yulskers::parser::ParseBlock, c...>;
	return check_for_failure_t<result>{};
}


template<typename Char, Char... c>
constexpr auto operator""_yulskers_allow_failure()
{
	return format_reason_if_failure_t<parser::parse<yulskers::parser::ParseBlock, c...>>{};
}

}
