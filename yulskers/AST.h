#pragma once

namespace yulskers::ast
{

template<char... cs>
struct Placeholder {};

template<typename... Statements>
struct Block {};

template<typename Variables, typename Value>
struct VariableDeclaration {};
template<typename Variables, typename Value>
struct Assignment {};
template<typename Condition, typename Block>
struct If {};
template<typename Name, typename Arguments, typename Returns, typename Body>
struct Function {};
struct Leave{};
template<typename Expression, typename... Cases>
struct Switch {};
template<typename Init, typename Condition, typename Post, typename Body>
struct For {};
struct Continue{};
struct Break{};

template<typename Kind>
struct Expression {};
template<typename Variable, typename Arguments>
struct FunctionCall {};
template<char... cs>
struct Identifier {};
template<typename Token>
struct Literal {};

namespace literals {
template<typename Char, Char... c>
constexpr auto operator""_yulskers_identifier() { return Identifier<c...>{}; }
template<typename Char, Char... c>
constexpr auto operator""_yulskers_placeholder() { return Placeholder<c...>{}; }
}

}
