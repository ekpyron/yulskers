#pragma once

#include <yulskers/parser/ParserBase.h>
#include <yulskers/parser/ParseBlock.h>

namespace yulskers::parser
{

namespace detail {
template<typename result, typename = void>
struct ParserStateResultAST { using type = result; };
template<typename result>
struct ParserStateResultAST<result, std::enable_if_t<!is_failure_v<result>>>
{
	static_assert(result::ast::size == 1);
	using type = type_list_first_t<typename result::ast>;
};
}

template<typename ParserStage, typename CharList>
struct Parse
{
	using type = typename detail::ParserStateResultAST<apply_op_t<
		typename ParserStateInit<CharList>::type,
		ParserStage
	>>::type;
};

template<typename ParserStage>
struct Parse<ParserStage, char_list<>>
{
	using type = decltype("Attempted to parse empty string."_yulskers_error);
};

template<typename ParserStage, char... c>
using parse = typename Parse<ParserStage, char_list<c...>>::type;

}
