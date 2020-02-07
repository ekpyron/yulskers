#pragma once

#include <yulskers/parser/ParserBase.h>
#include <yulskers/parser/ParseStatement.h>
#include <yulskers/Token.h>

namespace yulskers::parser {

template<typename AST>
struct MakeBlock;
template<typename... Statements>
struct MakeBlock<type_list<Statements...>>
{
	using type = ast::Block<Statements...>;
};


struct ParseBlock: parse_node_op<
	Expect<TokenKind::BracketOpen, Failure<decltype("expected {"_char_list)>>,
	repeat_op<ParseStatement>,
	Expect<TokenKind::BracketClose, Failure<decltype("expected statement or }"_char_list)>>,
	return_node<make_op<MakeBlock>>
> {};

}