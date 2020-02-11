#pragma once

#include <yulskers/parser/ParserBase.h>
#include <yulskers/parser/ParseIdentifier.h>
#include <yulskers/parser/ParseExpression.h>

namespace yulskers::parser {

template<typename Token, typename Peek, typename = void>
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
	WhileToken<TokenKind::Comma,
		advance_op,
		ParseIdentifier
	>,
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

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Let>>
{
	using type = parse_node_op<
		advance_op,
		ParseIdentifierList,
		IfToken<TokenKind::Assign, advance_op, ParseExpression>,
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

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::If>>
{
	using type = parse_node_op<
		advance_op,
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

template<typename Token, typename Peek, typename = void>
struct ParseCaseImpl
{
	using type = void;
};

template<typename Token, typename Peek>
struct ParseCaseImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Case>>
{
	using type = parse_node_op<
	    advance_op,
	    ParseLiteral,
	    ParseBlock,
		return_node<make_op<MakeTemporaryNodeList>>
	>;
};

template<typename Token, typename Peek>
struct ParseCaseImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Default>>
{
	using type = parse_node_op<
		advance_op,
		ParseBlock,
		return_node<make_op<MakeTemporaryNodeList>>
	>;
};

using ParseCase = parse_token_op<ParseCaseImpl>;

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Switch>>
{
	using type = parse_node_op<
		advance_op,
		ParseExpression,
		IfNotToken<TokenKind::Case, Expect<TokenKind::Default, Failure<decltype("Expected case or default case."_char_list)>, false>>,
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

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::For>>
{
	using type = parse_node_op<
		advance_op,
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

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Function>>
{
	using type = parse_node_op<
		advance_op,
		ParseIdentifier,
		parse_node_op<
			Expect<TokenKind::ParenOpen, Failure<decltype("expected ("_char_list)>>,
			IfNotToken<TokenKind::ParenClose, ParseIdentifierList>,
			Expect<TokenKind::ParenClose, Failure<decltype("expected )"_char_list)>>,
			return_node<make_op<MakeTemporaryNodeList>>
		>,
		parse_node_op<
		    IfToken<TokenKind::Arrow, advance_op, ParseIdentifierList>,
			return_node<make_op<MakeTemporaryNodeList>>
		>,
		ParseBlock,
		return_node<make_op<MakeFunction>>
	>;
};

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::BracketOpen>>
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
template<char... cs>
struct MakePlaceholder<token::Placeholder<cs...>>
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
	        advance_op,
	        return_node<MakePlaceholder<FirstToken>>
	    >,
		Failure<decltype("expected call or assignment"_char_list)>
	>;
};

template<typename Identifier, typename Token>
struct ParseCallOrAssignment<Identifier, Token, std::enable_if_t<
	!std::is_same_v<Token, void> && (
		token_traits::token_kind_v<Token> == TokenKind::Assign ||
		token_traits::token_kind_v<Token> == TokenKind::Comma
	)
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
struct ParseCallOrAssignment<Identifier, Token, std::enable_if_t<
	token_traits::token_kind_v<Token> == TokenKind::ParenOpen &&
	!std::is_same_v<Token, void>
>>
{
	using type = parse_node_op<
	    ParseIdentifier,
	    Expect<TokenKind::ParenOpen, Failure<decltype("expected ("_char_list)>>,
	    IfNotToken<TokenKind::ParenClose,
	    	ParseExpression,
	    	WhileToken<TokenKind::Comma,
	    		advance_op,
	    		ParseExpression
	    	>
	    >,
		Expect<TokenKind::ParenClose, Failure<decltype("Expected , or )."_char_list)>>,
		return_node<make_op<MakeFunctionCall>>
	>;
};

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<
	token_traits::token_kind_v<Token> == TokenKind::Identifier ||
	token_traits::token_kind_v<Token> == TokenKind::Placeholder
>>
{
	using type = parse_token_op<ParseCallOrAssignment>;
};

template<typename Node>
struct ParseSingleTokenStatement
{
	using type = parse_node_op<
	    advance_op,
	    return_node<const_op<Node>>
	>;
};

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Continue>>:
	ParseSingleTokenStatement<ast::Continue> {};

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Break>>:
	ParseSingleTokenStatement<ast::Break> {};

template<typename Token, typename Peek>
struct ParseStatementImpl<Token, Peek, std::enable_if_t<token_traits::token_kind_v<Token> == TokenKind::Leave>>:
	ParseSingleTokenStatement<ast::Leave> {};

using ParseStatement = parse_token_op<ParseStatementImpl>;

}
