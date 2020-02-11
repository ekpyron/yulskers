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
	using type = typename result::ast;
};
}

template<typename CharList>
struct Parse
{
	using type = typename detail::ParserStateResultAST<apply_op_t<
		typename ParserStateInit<CharList>::type,
		ParseBlock
	>>::type;
};
template<char... c>
using parse = typename Parse<char_list<c...>>::type;

}
