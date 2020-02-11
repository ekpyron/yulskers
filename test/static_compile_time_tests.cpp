#include <yulskers/yulskers.h>
#include <iostream>

using namespace yulskers;
using namespace yulskers::literals;
/*
#include <cxxabi.h>
template<typename T>
void printType()
{
	int status;
	char *result = __cxxabiv1::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
	std::cout << std::string(result) << std::endl;
	if constexpr(is_failure_v<T>)
		std::cout << T::message() << std::endl;
	free(result);
}
*/
template<typename Char, Char... c>
constexpr auto operator""_char_list() { return char_list<c...>{}; }

/// Failure cases

static_assert(std::is_same_v<
	decltype(""_yulskers_allow_failure),
	Failure<decltype("Attempted to parse empty string."_char_list)>
>);
static_assert(std::is_same_v<
	decltype("a"_yulskers_allow_failure),
	Failure<decltype("Expected {."_char_list), failure::WhileParsing<'a'>>
>);
static_assert(std::is_same_v<
	decltype("{("_yulskers_allow_failure),
	Failure<decltype("Expected statement or }."_char_list), failure::WhileParsing<'('>>
>);
static_assert(std::is_same_v<
	decltype("{ a( }"_yulskers_allow_failure),
	Failure<decltype("Expected expression."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{"_yulskers_allow_failure),
	Failure<decltype("Unexpected end of stream."_char_list), failure::WhileParsing<>>
>);
static_assert(std::is_same_v<
	decltype("{a"_yulskers_allow_failure),
	Failure<decltype("Expected call or assignment."_char_list), failure::WhileParsing<'a'>>
>);
static_assert(std::is_same_v<
	decltype("{ a(a }"_yulskers_allow_failure),
	Failure<decltype("Expected , or )."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ let }"_yulskers_allow_failure),
	Failure<decltype("Expected identifier."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ let a := }"_yulskers_allow_failure),
	Failure<decltype("Expected expression."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function }"_yulskers_allow_failure),
	Failure<decltype("Expected identifier."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f }"_yulskers_allow_failure),
	Failure<decltype("Expected (."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f( }"_yulskers_allow_failure),
	Failure<decltype("Expected identifier."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f() }"_yulskers_allow_failure),
	Failure<decltype("Expected {."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f(a }"_yulskers_allow_failure),
	Failure<decltype("Expected )."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f(a, }"_yulskers_allow_failure),
	Failure<decltype("Expected identifier."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f(a, b }"_yulskers_allow_failure),
	Failure<decltype("Expected )."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f(a, b) }"_yulskers_allow_failure),
	Failure<decltype("Expected {."_char_list), failure::WhileParsing<'}'>>
>);
static_assert(std::is_same_v<
	decltype("{ function f(a, b) { )"_yulskers_allow_failure),
	Failure<decltype("Expected statement or }."_char_list), failure::WhileParsing<')'>>
>);
static_assert(std::is_same_v<
	decltype("/*"_yulskers_allow_failure),
	Failure<decltype("Unterminated comment."_char_list), failure::WhileParsing<'/', '*'>>
>);
static_assert(std::is_same_v<
	decltype("{ 0xG"_yulskers_allow_failure),
	Failure<decltype("Expcted hex digit."_char_list), failure::WhileParsing<'0', 'x', 'G'>>
>);
// TODO: this should rather be "Expected decimal digit.", but is not correctly implemented in the scanner.
static_assert(std::is_same_v<
	decltype("{ 0y"_yulskers_allow_failure),
	Failure<decltype("Expected statement or }."_char_list), failure::WhileParsing<'0', 'y'>>
>);
static_assert(std::is_same_v<
	decltype("%"_yulskers_allow_failure),
	Failure<decltype("Unexpected character."_char_list), failure::WhileParsing<'%'>>
>);
static_assert(std::is_same_v<
	decltype("{ \""_yulskers_allow_failure),
	Failure<decltype("Unterminated string literal."_char_list), failure::WhileParsing<'\"'>>
>);
static_assert(std::is_same_v<
	decltype("{ hex"_yulskers_allow_failure),
	Failure<decltype("Expected hex string literal."_char_list), failure::WhileParsing<'h', 'e', 'x'>>
>);
static_assert(std::is_same_v<
	decltype("{ hex'"_yulskers_allow_failure),
	Failure<decltype("Invalid or unterminated hex string literal."_char_list), failure::WhileParsing<'h', 'e', 'x', '\''>>
>);
static_assert(std::is_same_v<
	decltype("{ hex'0"_yulskers_allow_failure),
	Failure<decltype("Invalid or unterminated hex string literal."_char_list), failure::WhileParsing<'h', 'e', 'x', '\'', '0'>>
>);
static_assert(std::is_same_v<
	decltype("{ hex'0'"_yulskers_allow_failure),
	Failure<decltype("Invalid or unterminated hex string literal."_char_list), failure::WhileParsing<'h', 'e', 'x', '\'', '0', '\''>>
>);

/// ast::Block
static_assert(std::is_same_v<
    decltype("{}"_yulskers_block),
    ast::Block<>
>);
static_assert(std::is_same_v<
	decltype("{{}}"_yulskers_block),
	ast::Block<ast::Block<>>
>);
static_assert(std::is_same_v<
	decltype("{ { } { } }"_yulskers_block),
	ast::Block<ast::Block<>, ast::Block<>>
>);
static_assert(std::is_same_v<
	decltype("{ { {} } { } }"_yulskers_block),
	ast::Block<ast::Block<ast::Block<>>, ast::Block<>>
>);

int main()
{
//	printType<decltype("{}"_yulskers_block)>();
	return EXIT_SUCCESS;
}
