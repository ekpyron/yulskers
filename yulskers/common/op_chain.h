/**
 * Generic operations on a state and chains of operations on a state.
 *
 * An operation Op on a state can be:
 *  - a class with a member template ``template<typename State> struct apply`` definiing the operation.
 *  - a class with a single member ``type`` defining the result of the operation.
 *  - a failure type ``Failure<Reason>``
 *  - ``void`` defining a no-operation
 *
 *  An operation can be applied to a state using ``apply_op_t<State, Op>``, which evaluates to
 *  - the result of the operation, if it has one
 *  - the returned failure type, if the operation failed
 *  - ``State``, if the operation is a no-operation (i.e. if the operation is ``void``)
 *
 *  The helper ``make_op<TemplateOp>`` converts a single argument template ``TemplateOp`` to an operation, which
 *  returns ``TemplateOp<State>::type``.
 *  The helper ``const_op<Result>`` creates an operation that always returns ``Result``.
 *
 * ``op_chain<Ops...>`` defines an operation which sequentially applies all operations in ``Ops...``.
 *  - Each operation in ``Ops...`` can be one of the following:
 *    - A regular operation as defined above.
 *    - ``repeat_op<Op>`` which results in the repeated execution of ``Op``, until its result is ``void`` or an ``op_chain_break``.
 *    - ``op_chain_break`` which terminates the op chain returning the last state
 * - If an operation results in ``Failure<Reason>`` the chain aborts and returns the failure.
 * - If an operation results in ``void`` the chain continues at the last state.
 *
 */

#pragma once

namespace yulskers {

template<typename State, typename Op, typename = void>
struct apply_op
{
	using type = typename Op::type;
};
template<typename State, typename Op>
struct apply_op<State, Op, std::enable_if_t<is_failure_v<Op> || std::is_same_v<Op, void>>>
{
	using type = std::conditional_t<std::is_same_v<Op, void>, State, Op>;
};
template<typename State, typename Op>
struct apply_op<State, Op, std::void_t<typename Op::template apply<State>>>
{
	using type = typename Op::template apply<State>::type;
};
template<typename State, typename Op>
using apply_op_t = typename apply_op<State, Op>::type;

template<template<typename> typename TemplateOp>
struct make_op
{
	template<typename State>
	using apply = TemplateOp<State>;
};

template<typename Result>
struct const_op
{
	using type = Result;
};

template<typename T>
struct repeat_op { using type = T; };

struct op_chain_break { using type = op_chain_break; };

namespace detail {

	template<typename OrigState, typename Result, typename Tail>
	struct op_chain_step_result
	{
		using result = Result;
		using tail = Tail;
	};
	template<typename OrigState, typename Reason, typename Tail>
	struct op_chain_step_result<OrigState, Failure<Reason>, Tail>
	{
		using result = Failure<Reason>;
		using tail = type_list<>;
	};
	template<typename OrigState, typename Tail>
	struct op_chain_step_result<OrigState, void, Tail>
	{
		using result = OrigState;
		using tail = Tail;
	};
	template<typename OrigState, typename Tail>
	struct op_chain_step_result<OrigState, op_chain_break, Tail>
	{
		using result = OrigState;
		using tail = type_list<>;
	};
	template<typename State, typename Step, typename list, typename = void>
	struct op_chain_apply_step
	{
		using stepResult = apply_op_t<State, Step>;
		using type = op_chain_step_result<
			State,
			std::conditional_t<std::is_same_v<stepResult, void>, State, stepResult>,
			list
		>;
	};
	template<typename State, typename Step, typename list>
	struct op_chain_apply_step<State, repeat_op<Step>, list>
	{
		using stepResult = apply_op_t<State, Step>;
		using type = op_chain_step_result<
			State,
			stepResult,
			std::conditional_t<std::is_same_v<stepResult, State>, list, type_list_cons_t<repeat_op<Step>, list>>
		>;
	};

	template<typename State, typename list>
	struct op_chain_impl
	{
		using step = typename detail::op_chain_apply_step<State, type_list_first_t<list>, type_list_drop_first_t<list>>::type;
		using type = typename op_chain_impl<typename step::result, typename step::tail>::type;
	};
	template<typename State>
	struct op_chain_impl<State, type_list<>>
	{
		using type = State;
	};
}

template<typename... Ops>
struct op_chain
{
	template<typename State>
	struct apply
	{
		using type = typename detail::op_chain_impl<State, type_list<Ops...>>::type;
	};
};

}