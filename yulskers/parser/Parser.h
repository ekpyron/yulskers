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

template<typename TokenList>
struct Parse
{
	using type = typename detail::ParserStateResultAST<apply_op_t<
		ParserStateInit<TokenList>,
		ParseBlock
	>>::type;
};
template<typename TokenList>
using parse = typename Parse<TokenList>::type;

}
