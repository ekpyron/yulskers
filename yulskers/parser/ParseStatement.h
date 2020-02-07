#pragma once

#include <yulskers/parser/ParserBase.h>
#include <yulskers/parser/ParseIdentifier.h>
#include <yulskers/parser/ParseExpression.h>

namespace yulskers::parser {

template<typename Token, typename = void>
struct ParseStatementImpl
{
	using type = void;
};


template<typename AST>
struct MakeTemporaryNodeList;
template<typename... Identifiers>
struct MakeTemporaryNodeList<type_list<Identifiers...>>
{
	using type = type_list<Identifiers...>;
};

using ParseIdentifierList = parse_node_op<
	ParseIdentifier,
	repeat_op<op_chain<
		Expect<TokenKind::Comma, op_chain_break>,
		ParseIdentifier
	>>,
	return_node<make_op<MakeTemporaryNodeList>>
>;

template<typename AST>
struct MakeVariableDeclaration;
template<typename LHS, typename RHS>
struct MakeVariableDeclaration<type_list<LHS, RHS>>
{
	using type = ast::VariableDeclaration<LHS, RHS>;
};
template<typename LHS>
struct MakeVariableDeclaration<type_list<LHS>>
{
	using type = ast::VariableDeclaration<LHS, void>;
};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Let>>
{
	using type = parse_node_op<
		pop_tokens_op<1>,
		ParseIdentifierList,
		op_chain<
		    Expect<TokenKind::Assign, op_chain_break>,
			ParseExpression
		>,
		return_node<make_op<MakeVariableDeclaration>>
	>;
};

template<typename AST>
struct MakeIf;

template<typename Condition, typename Block>
struct MakeIf<type_list<Condition, Block>>
{
	using type = ast::If<Condition, Block>;
};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::If>>
{
	using type = parse_node_op<
		pop_tokens_op<1>,
		ParseExpression,
		ParseBlock,
		return_node<make_op<MakeIf>>
	>;
};

template<typename AST>
struct MakeSwitch;
template<typename Expression, typename... Cases>
struct MakeSwitch<type_list<Expression, Cases...>>
{
	// TODO: disallow multiple default cases and multiple cases with same literal value
	using type = ast::Switch<Expression, Cases...>;
};

template<typename Token, typename = void>
struct ParseCaseImpl
{
	using type = void;
};

template<typename Token>
struct ParseCaseImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Case>>
{
	using type = parse_node_op<
	    pop_tokens_op<1>,
	    ParseLiteral,
	    ParseBlock,
		return_node<make_op<MakeTemporaryNodeList>>
	>;
};

template<typename Token>
struct ParseCaseImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Default>>
{
	using type = parse_node_op<
		pop_tokens_op<1>,
		ParseBlock,
		return_node<make_op<MakeTemporaryNodeList>>
	>;
};

using ParseCase = parse_token_op<ParseCaseImpl, 1, 0>;

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Switch>>
{
	using type = parse_node_op<
		pop_tokens_op<1>,
		ParseExpression,
		Expect<TokenKind::Case, Expect<TokenKind::Default, Failure<decltype("Expected case or default case."_char_list)>, false>, false>,
		repeat_op<ParseCase>,
		return_node<make_op<MakeSwitch>>
	>;
};

template<typename AST>
struct MakeFor;

template<typename Init, typename Condition, typename Post, typename Body>
struct MakeFor<type_list<Init, Condition, Post, Body>>
{
	using type = ast::For<Init, Condition, Post, Body>;
};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::For>>
{
	using type = parse_node_op<
		pop_tokens_op<1>,
		ParseBlock,
		ParseExpression,
		ParseBlock,
		ParseBlock,
		return_node<make_op<MakeFor>>
	>;
};


template<typename AST>
struct MakeFunction;
namespace detail {
	template<typename List>
	struct UnpackList;
	template<typename List>
	struct UnpackList<type_list<List>> { using type = List; };
	template<>
	struct UnpackList<type_list<>> { using type = type_list<>; };
}
template<typename Name, typename Arguments, typename Returns, typename Body>
struct MakeFunction<type_list<Name, Arguments, Returns, Body>>
{
	using type = ast::Function<Name, typename detail::UnpackList<Arguments>::type, typename detail::UnpackList<Returns>::type, Body>;
};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Function>>
{
	using type = parse_node_op<
		pop_tokens_op<1>,
		ParseIdentifier,
		parse_node_op<
			Expect<TokenKind::ParenOpen, Failure<decltype("expected ("_char_list)>>,
			Expect<TokenKind::ParenClose, ParseIdentifierList, false>,
			Expect<TokenKind::ParenClose, Failure<decltype("expected )"_char_list)>>,
			return_node<make_op<MakeTemporaryNodeList>>
		>,
		parse_node_op<
			op_chain<
				Expect<TokenKind::Arrow, op_chain_break>,
				ParseIdentifierList
			>,
			return_node<make_op<MakeTemporaryNodeList>>
		>,
		ParseBlock,
		return_node<make_op<MakeFunction>>
	>;
};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::BracketOpen>>
{
	using type = parse_node_op<ParseBlock>;
};

template<typename AST>
struct MakeAssignment;
template<typename LHS, typename RHS>
struct MakeAssignment<type_list<LHS, RHS>>
{
	using type = ast::Assignment<LHS, RHS>;
};

template<typename Placeholder>
struct MakePlaceholder;
template<char... cs, std::size_t start, std::size_t end>
struct MakePlaceholder<Token<token::Placeholder<cs...>, start, end>>
{
	template<typename State>
	struct apply {
		using type = ast::Placeholder<cs...>;
	};
};

template<typename FirstToken, typename SecondToken, typename = void>
struct ParseCallOrAssignment
{
	using type = std::conditional_t<
	    token_traits::token_kind_v<FirstToken> == TokenKind::Placeholder,
	    parse_node_op<
	        pop_tokens_op<1>,
	        return_node<MakePlaceholder<FirstToken>>
	    >,
		Failure<decltype("expected call or assignment"_char_list)>
	>;
};

template<typename Identifier, typename Token>
struct ParseCallOrAssignment<Identifier, Token, std::enable_if_t<
	token_traits::token_kind_v<Token> == TokenKind::Assign ||
	token_traits::token_kind_v<Token> == TokenKind::Comma
>>
{
	using type = parse_node_op<
		ParseIdentifierList,
		Expect<TokenKind::Assign, Failure<decltype("expected :="_char_list)>>,
		ParseExpression,
		return_node<make_op<MakeAssignment>>
	>;
};

template<typename Identifier, typename Token>
struct ParseCallOrAssignment<Identifier, Token, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::ParenOpen>>
{
	using type = parse_node_op<
	    ParseIdentifier,
	    Expect<TokenKind::ParenOpen, Failure<decltype("expected ("_char_list)>>,
		Expect<TokenKind::ParenClose, op_chain<
			ParseExpression,
			repeat_op<op_chain<
				Expect<TokenKind::Comma, op_chain_break>,
				ParseExpression
			>>
		>, false>,
		Expect<TokenKind::ParenClose, Failure<decltype("Expected , or )."_char_list)>>,
		return_node<make_op<MakeFunctionCall>>
	>;
};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<
	token_traits::token_kind_v<Token> == TokenKind::Identifier ||
	token_traits::token_kind_v<Token> == TokenKind::Placeholder
>>
{
	using type = parse_token_op<ParseCallOrAssignment, 2, 0>;
};

template<typename Node>
struct ParseSingleTokenStatement
{
	using type = parse_node_op<
	    pop_tokens_op<1>,
	    return_node<const_op<Node>>
	>;
};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<std::is_same_v<typename Token::type, token::Continue>>>:
	ParseSingleTokenStatement<ast::Continue> {};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<std::is_same_v<typename Token::type, token::Break>>>:
	ParseSingleTokenStatement<ast::Break> {};

template<typename Token>
struct ParseStatementImpl<Token, std::enable_if_t<std::is_same_v<typename Token::type, token::Leave>>>:
	ParseSingleTokenStatement<ast::Leave> {};

using ParseStatement = parse_token_op<ParseStatementImpl, 1, 0>;

}
