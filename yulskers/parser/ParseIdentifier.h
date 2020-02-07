#pragma once

#include <yulskers/parser/ParserBase.h>

namespace yulskers::parser {

template<typename Token>
struct ParseIdentifierImpl
{
	using type = Failure<decltype("Expected identifier"_char_list)>;
};

template<char... cs, size_t start, size_t end>
struct ParseIdentifierImpl<Token<token::Identifier<cs...>, start, end>>
{
	using type = parse_node_op<return_node<const_op<ast::Identifier < cs...>>>>;
};

template<char... cs, size_t start, size_t end>
struct ParseIdentifierImpl<Token<token::Placeholder<cs...>, start, end>>
{
	using type = parse_node_op<return_node<const_op<ast::Placeholder<cs...>>>>;
};

using ParseIdentifier = parse_token_op<ParseIdentifierImpl, 1, 1>;

}