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
	Expect<TokenKind::BracketOpen, decltype("Expected {."_yulskers_error)>,
	repeat_op<ParseStatement>,
	Expect<TokenKind::BracketClose, decltype("Expected statement or }."_yulskers_error)>,
	return_node<make_op<MakeBlock>>
> {};

}