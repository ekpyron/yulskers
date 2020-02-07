#pragma once

#include <yulskers/common.h>

namespace yulskers::parser {

template<typename TokenList, typename AST>
struct ParserState
{
	using tokens = TokenList;
	using ast = AST;
};

template<typename TokenList>
using ParserStateInit = ParserState<drop_comment_tokens_t<drop_whitespace_tokens_t<TokenList>>, type_list<>>;

using EmptyAST = type_list<>;

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

template<typename State, std::size_t N>
struct pop_tokens;
template<typename State, std::size_t N>
using pop_tokens_t = typename pop_tokens<State, N>::type;
template<typename TokenList, typename AST, std::size_t N>
struct pop_tokens<ParserState<TokenList, AST>, N>
{
	using type = ParserState<type_list_drop_first_t<TokenList, N>, AST>;
};

template<typename State, std::size_t N>
struct nth_token;
template<typename State, std::size_t N>
using nth_token_t = typename nth_token<State, N>::type;
template<typename TokenList, typename AST, std::size_t N>
struct nth_token<ParserState<TokenList, AST>, N>
{
	using type = type_list_get_t<TokenList, N>;
};

template<typename State, std::size_t N, typename = void>
struct nth_token_or_void { using type = void; };
template<typename State, std::size_t N>
using nth_token_or_void_t = typename nth_token_or_void<State, N>::type;
template<typename State, std::size_t N>
struct nth_token_or_void<State, N, std::void_t<typename nth_token<State, N>::type>>
{
	using type = nth_token_t<State, N>;
};

template<typename State>
struct num_tokens;
template<typename TokenList, typename AST>
struct num_tokens<ParserState<TokenList, AST>>
{
	static constexpr auto value = TokenList::size;
};
template<typename State>
static constexpr auto num_tokens_v = num_tokens<State>::value;


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

template<typename State>
struct current_token { using type = void; };
template<typename TokenList, typename AST>
struct current_token<ParserState<TokenList, AST>> {
	static_assert(num_tokens_v<ParserState<TokenList, AST>> > 0, "Unexpected end of stream.");
	using type = type_list_first_t<TokenList>;
};
template<typename State>
using current_token_t = typename current_token<State>::type;

template<typename State>
struct next_token: Failure<decltype("expected more tokens"_char_list)> {};

template<typename TokenList, typename AST>
struct next_token<ParserState<TokenList, AST>>
{
	using type = ParserState<type_list_drop_first_t<TokenList>, AST>;
};

template<typename State>
using next_token_t = typename next_token<State>::type;
}
