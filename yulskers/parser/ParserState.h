#pragma once

#include <yulskers/common.h>

namespace yulskers::parser {

namespace detail {
	template<typename Scanner, typename = void>
	struct getCurrentToken { using type = void; };
	template<typename Scanner>
	struct getCurrentToken<Scanner, std::void_t<typename Scanner::type>>
	{
		using type = typename Scanner::type;
	};
	template<typename Scanner, typename = void>
	struct getNextScanner { using type = void; };
	template<typename Scanner>
	struct getNextScanner<Scanner, std::void_t<typename Scanner::next>>
	{
		using type = typename Scanner::next;
	};
	template<typename Scanner, typename = void>
	struct hasTokensLeft { static constexpr bool value = false; };
	template<typename Scanner>
	struct hasTokensLeft<Scanner, std::void_t<typename Scanner::type>> { static constexpr bool value = !std::is_same_v<typename Scanner::type, void>; };
}

template<typename Scanner, typename AST>
struct ParserState
{
	using currentToken = typename detail::getCurrentToken<Scanner>::type;
	using nextScanner = typename detail::getNextScanner<Scanner>::type;
	using nextToken = typename detail::getCurrentToken<nextScanner>::type;
	static constexpr bool hasTokensLeft = detail::hasTokensLeft<Scanner>::value;//!std::is_same_v<typename Scanner::next, void>;
	using ast = AST;
};


template<typename State>
using advance_t = ParserState<typename State::nextScanner, typename State::ast>;
template<typename State>
using current_token_t = typename State::currentToken;
template<typename State>
using next_token_or_void_t = typename State::nextToken;
template<typename State>
static constexpr bool has_token_left_v = State::hasTokensLeft;

using EmptyAST = type_list<>;

template<typename CharList>
struct ParserStateInit
{
	using type = ParserState<scanner::Scanner<CharList>, EmptyAST>;
};

template<typename State>
struct parser_current_ast;
template<typename State>
using parser_current_ast_t = typename parser_current_ast<State>::type;
template<typename TokenList, typename AST>
struct parser_current_ast<ParserState<TokenList, AST>>
{
	using type = AST;
};

template<typename State, typename AST>
struct parser_append_ast;
template<typename State, typename AST>
using parser_append_ast_t = typename parser_append_ast<State, AST>::type;
template<typename TokenList, typename OldAST, typename AppendAST>
struct parser_append_ast<ParserState<TokenList, OldAST>, AppendAST>
{
	using type = ParserState<TokenList, type_list_concat_t<OldAST, AppendAST>>;
};

template<typename State, typename AST>
struct parser_replace_ast;
template<typename State, typename AST>
using parser_replace_ast_t = typename parser_replace_ast<State, AST>::type;
template<typename TokenList, typename OldAST, typename NewAST>
struct parser_replace_ast<ParserState<TokenList, OldAST>, NewAST>
{
	using type = ParserState<TokenList, NewAST>;
};


template<typename State>
struct num_nodes;
template<typename TokenList, typename... Nodes>
struct num_nodes<ParserState<TokenList, type_list<Nodes...>>>: std::integral_constant<std::size_t, sizeof...(Nodes)> {};
template<typename State>
static constexpr auto num_nodes_v = num_nodes<State>::value;

template<typename State, typename Node>
struct push_node
{
	using type = parser_append_ast_t<State, type_list<Node>>;
};
template<typename State, typename Node>
using push_node_t = typename push_node<State, Node>::type;



}
