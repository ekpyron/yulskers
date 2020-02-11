#pragma once

#include <yulskers/parser/ParserBase.h>
#include <yulskers/parser/ParseIdentifier.h>

namespace yulskers::parser {

template<typename Token, typename Peek, typename = void>
struct ParseExpressionImpl
{
	using type = decltype("Expected expression."_yulskers_error);
};

using ParseExpression = parse_token_op<ParseExpressionImpl>;

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

template<typename Token, typename Peek>
struct ParseExpressionImpl<Token, Peek, std::enable_if_t<
	token_traits::token_kind_v<Token> == TokenKind::Identifier ||
	token_traits::token_kind_v<Token> == TokenKind::Placeholder
>>
{
	using type = parse_node_op<
	    ParseIdentifier,
	    IfToken<TokenKind::ParenOpen,
			advance_op,
			IfNotToken<TokenKind::ParenClose,
					ParseExpression,
					WhileToken<TokenKind::Comma,
						advance_op,
						ParseExpression
					>
			>,
			Expect<TokenKind::ParenClose, decltype("Expected , or )."_yulskers_error)>,
			return_node<make_op<MakeFunctionCall>>
	    >,
	    return_node<make_op<MakeExpression>>
	>;
};

template<typename Token, typename Peek, typename = void>
struct ParseLiteralImpl
{
	using type = decltype("Expected literal."_yulskers_error);
};
template<typename Token, typename Peek>
struct ParseLiteralImpl<Token, Peek, std::enable_if_t<token_traits::is_literal_v<Token>>>
{
	using type = parse_node_op<
	    advance_op,
		return_node<const_op<ast::Literal<token_traits::token_type_t<Token>>>>
	>;
};
template<char... cs, typename Peek>
struct ParseLiteralImpl<token::Placeholder<cs...>, Peek>
{
	using type = parse_node_op<advance_op, return_node<const_op<ast::Literal<ast::Placeholder<cs...>>>>>;
};
using ParseLiteral = parse_token_op<ParseLiteralImpl>;

template<typename Token, typename Peek>
struct ParseExpressionImpl<Token, Peek, std::enable_if_t<token_traits::is_literal_v<Token>>>
{
	using type = parse_node_op<
	    ParseLiteral,
	    return_node<make_op<MakeExpression>>
	>;
};

}