#pragma once

#include <yulskers/parser/ParserBase.h>

namespace yulskers::parser {

template<typename Token, typename Peek>
struct ParseIdentifierImpl
{
	using type = Failure<decltype("Expected identifier"_char_list)>;
};

template<char... cs, typename Peek>
struct ParseIdentifierImpl<token::Identifier<cs...>, Peek>
{
	using type = parse_node_op<advance_op, return_node<const_op<ast::Identifier < cs...>>>>;
};

template<char... cs, typename Peek>
struct ParseIdentifierImpl<token::Placeholder<cs...>, Peek>
{
	using type = parse_node_op<advance_op, return_node<const_op<ast::Placeholder<cs...>>>>;
};

using ParseIdentifier = parse_token_op<ParseIdentifierImpl>;

}