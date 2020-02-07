#pragma once

#include <yulskers/parser/ParserState.h>
#include <yulskers/AST.h>
#include <yulskers/Token.h>
#include <yulskers/common/op_chain.h>

namespace yulskers::parser
{

namespace detail {
	template<typename State, template<typename...> typename Op, typename ElemList, std::size_t Consume, typename = void>
	struct parse_token_op_impl:
		Failure<decltype("Unexpected end of stream."_char_list)> {};
	template<typename State, template<typename...> typename Op, std::size_t...N, std::size_t Consume>
	struct parse_token_op_impl<State, Op, std::index_sequence<N...>, Consume, std::enable_if_t<num_tokens_v<State> >= sizeof...(N)>> {
		using type = apply_op<pop_tokens_t<State, Consume>, typename Op<nth_token_t<State, N>...>::type>;
	};
}

template<template<typename...> typename Op, std::size_t Peek, std::size_t Consume>
struct parse_token_op
{
	template<typename State>
	using apply = typename detail::parse_token_op_impl<State, Op, std::make_index_sequence<Peek>, Consume>::type;
};

namespace detail {

struct parse_node_begin
{
	template<typename State>
	struct apply
	{
		using type = parser_replace_ast_t<State, EmptyAST>;
	};
};

template<typename OldAST>
struct parse_node_finalize
{
	template<typename State>
	struct apply
	{
		using type = parser_replace_ast_t<State, type_list_concat_t<OldAST, parser_current_ast_t<State>>>;
	};
};
}

template<typename... ParseOps>
struct parse_node_op
{
	template<typename State>
	struct apply
	{
		using type = apply_op_t<State, op_chain<
			detail::parse_node_begin,
			ParseOps...,
			detail::parse_node_finalize<parser_current_ast_t<State>>
		>>;
	};
};

namespace detail {
template<typename State, TokenKind kind, typename Otherwise, bool advance, typename = void>
struct ExpectImpl { using type = Otherwise; };
template<typename State, TokenKind kind, typename Otherwise, bool advance>
struct ExpectImpl<State, kind, Otherwise, advance, std::void_t<typename current_token_t<State>::type>>
: std::conditional<
	token_traits::token_kind_v<current_token_t<State>> == kind,
	std::conditional_t<advance, pop_tokens_t<State, 1>, State>,
	Otherwise
> {};

}

template<TokenKind kind, typename Otherwise, bool advance = true>
struct Expect
{
	template<typename State>
	using apply = detail::ExpectImpl<State, kind, apply_op_t<State, Otherwise>, advance>;
};

template<std::size_t N = 1>
struct pop_tokens_op
{
	template<typename State>
	struct apply
	{
		using type = pop_tokens_t<State, N>;
	};
};

template<typename Op>
struct return_node
{
	template<typename State>
	struct apply
	{
		using node = apply_op_t<parser_current_ast_t<State>, Op>;
		using type = parser_replace_ast_t<State, type_list<node>>;
	};
};

struct ParseBlock;

}
