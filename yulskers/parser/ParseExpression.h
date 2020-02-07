#pragma once

#include <yulskers/parser/ParserBase.h>
#include <yulskers/parser/ParseIdentifier.h>

namespace yulskers::parser {

template<typename Token, typename = void>
struct ParseExpressionImpl
{
	using type = Failure<decltype("expected expression"_char_list)>;
};

using ParseExpression = parse_token_op<ParseExpressionImpl, 1, 0>;

template<typename AST>
struct MakeFunctionCall;
template<typename Expression, typename... Arguments>
struct MakeFunctionCall<type_list<Expression, Arguments...>>
{
	using type = ast::FunctionCall<Expression, type_list<Arguments...>>;
};

template<typename AST>
struct MakeExpression;
template<typename Expression>
struct MakeExpression<type_list<Expression>>
{
	using type = ast::Expression<Expression>;
};

template<typename Token>
struct ParseExpressionImpl<Token, std::enable_if_t<
	token_traits::token_kind_v<Token> == TokenKind::Identifier ||
	token_traits::token_kind_v<Token> == TokenKind::Placeholder
>>
{
	using type = parse_node_op<
	    ParseIdentifier,
	    op_chain<
	        Expect<TokenKind::ParenOpen, op_chain_break>,
				Expect<TokenKind::ParenClose, op_chain<
						ParseExpression,
						repeat_op<op_chain<
						    Expect<TokenKind::Comma, op_chain_break>,
						    ParseExpression
						>>
					>, false>,
			Expect<TokenKind::ParenClose, Failure<decltype("Expected , or )."_char_list)>>,
			return_node<make_op<MakeFunctionCall>>
	    >,
	    return_node<make_op<MakeExpression>>
	>;
};

template<typename Token, typename = void>
struct ParseLiteralImpl
{
	using type = Failure<decltype("expected literal"_char_list)>;
};
template<typename Token>
struct ParseLiteralImpl<Token, std::enable_if_t<token_traits::is_literal_v<Token>>>
{
	using type = parse_node_op<
		return_node<const_op<ast::Literal<typename token_traits::token_type<Token>::type>>>
	>;
};
template<char... cs, size_t start, size_t end>
struct ParseLiteralImpl<Token<token::Placeholder<cs...>, start, end>>
{
	using type = parse_node_op<return_node<const_op<ast::Literal<ast::Placeholder<cs...>>>>>;
};
using ParseLiteral = parse_token_op<ParseLiteralImpl, 1, 1>;

template<typename Token>
struct ParseExpressionImpl<Token, std::enable_if_t<token_traits::is_literal_v<Token>>>
{
	using type = parse_node_op<
	    ParseLiteral,
	    return_node<make_op<MakeExpression>>
	>;
};

}