#pragma once

#include <yulskers/parser/ParserState.h>
#include <yulskers/AST.h>
#include <yulskers/Token.h>
#include <yulskers/common/op_chain.h>

namespace yulskers::parser
{

namespace detail {
	template<typename State, template<typename...> typename Op, typename = void>
	struct parse_token_op_impl:
		Failure<decltype("Unexpected end of stream."_char_list)> {};
	template<typename State, template<typename...> typename Op>
	struct parse_token_op_impl<State, Op, std::enable_if_t<has_token_left_v<State>>> {
		using type = apply_op<State, typename Op<current_token_t<State>, next_token_or_void_t<State>>::type>;
	};
}

struct advance_op
{
	template<typename State>
	struct apply
	{
		using type = advance_t<State>;
	};
};

template<template<typename...> typename Op>
struct parse_token_op
{
	template<typename State>
	using apply = typename detail::parse_token_op_impl<State, Op>::type;
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
struct ExpectImpl<State, kind, Otherwise, advance, std::enable_if_t<token_traits::token_kind_v<current_token_t<State>> == kind>>
: std::conditional<advance, advance_t<State>, State> {};

}

template<TokenKind kind, typename Otherwise, bool advance = true>
struct Expect
{
	template<typename State>
	using apply = detail::ExpectImpl<State, kind, apply_op_t<State, Otherwise>, advance>;
};

template<TokenKind tokenKind, typename... Op>
using IfToken = op_chain<Expect<tokenKind, op_chain_break, false>,	Op...>;

template<TokenKind tokenKind, typename... Op>
using IfNotToken = Expect<tokenKind, op_chain<Op...>, false>;

template<TokenKind tokenKind, typename... Op>
using WhileToken = repeat_op<op_chain<Expect<tokenKind, op_chain_break, false>, Op...>>;


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
