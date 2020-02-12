#include <yulskers/yulskers.h>

using namespace yulskers;
using namespace yulskers::literals;
using namespace yulskers::ast::literals;

template<typename Char, Char... c>
constexpr auto operator""_char_list() { return char_list<c...>{}; }

// Identifiers

static_assert(std::is_same_v<
    decltype("x"_yulskers_expression),
	ast::Expression<ast::Identifier<'x'>>
>);
static_assert(std::is_same_v<
	decltype("xyz"_yulskers_expression),
	ast::Expression<ast::Identifier<'x','y','z'>>
>);
static_assert(std::is_same_v<
	decltype("complex.identifier$name_42"_yulskers_expression),
	ast::Expression<ast::Identifier<
	    'c', 'o', 'm', 'p', 'l', 'e', 'x', '.',
	    'i', 'd', 'e', 'n', 't', 'i', 'f', 'i', 'e', 'r', '$', 'n', 'a', 'm', 'e', '_', '4', '2'
	>>
>);
static_assert(std::is_same_v<
	decltype("complex.identifier$name_42"_yulskers_expression),
	ast::Expression<decltype("complex.identifier$name_42"_yulskers_identifier)>
>);

// Function calls

static_assert(std::is_same_v<
	decltype("f()"_yulskers_expression),
	ast::Expression<ast::FunctionCall<ast::Identifier<'f'>, type_list<>>>
>);
static_assert(std::is_same_v<
	decltype("f(x)"_yulskers_expression),
	ast::Expression<ast::FunctionCall<ast::Identifier<'f'>, type_list<ast::Expression<ast::Identifier<'x'>>>>>
>);
static_assert(std::is_same_v<
	decltype("f(x, y)"_yulskers_expression),
	ast::Expression<ast::FunctionCall<ast::Identifier<'f'>, type_list<
		ast::Expression<ast::Identifier<'x'>>, ast::Expression<ast::Identifier<'y'>>
	>>>
>);

// Literals

// Decimal
static_assert(std::is_same_v<
	decltype("0"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<0>>>
>);
static_assert(std::is_same_v<
	decltype("1"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<1>>>
>);
static_assert(std::is_same_v<
	decltype("9"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<9>>>
>);
static_assert(std::is_same_v<
	decltype("10"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<1,0>>>
>);
static_assert(std::is_same_v<
	decltype("11"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<1,1>>>
>);
static_assert(std::is_same_v<
	decltype("19"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<1,9>>>
>);
static_assert(std::is_same_v<
	decltype("20"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<2,0>>>
>);
static_assert(std::is_same_v<
	decltype("1000000000000000000000000000000000000000000"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<
	    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	>>>
>);
static_assert(std::is_same_v<
	decltype("12345678901234567890123456789012345678901234567890123456789"
		     "0123456789012345678901234567890123456789012345678901234567890123456789"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<
	    1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,
	    1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,
	    1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9
	>>>
>);
static_assert(std::is_same_v<
	decltype("00001"_yulskers_expression),
	ast::Expression<ast::Literal<token::DecimalNumber<0,0,0,0,1>>>
>);

// Hexadecimal
static_assert(std::is_same_v<
	decltype("0x0"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<0>>>
>);
static_assert(std::is_same_v<
	decltype("0x1"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<1>>>
>);
static_assert(std::is_same_v<
	decltype("0x9"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<9>>>
>);
static_assert(std::is_same_v<
	decltype("0xA"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<10>>>
>);
static_assert(std::is_same_v<
	decltype("0xa"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<10>>>
>);
static_assert(std::is_same_v<
	decltype("0xF"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<15>>>
>);
static_assert(std::is_same_v<
	decltype("0xf"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<15>>>
>);
static_assert(std::is_same_v<
	decltype("0x10"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<1,0>>>
>);
static_assert(std::is_same_v<
	decltype("0x1F"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<1,15>>>
>);
static_assert(std::is_same_v<
	decltype("0x1f"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<1,15>>>
>);
static_assert(std::is_same_v<
	decltype("0xFFff"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexNumber<15, 15, 15, 15>>>
>);

// Hex strings
static_assert(std::is_same_v<
	decltype("hex'00'"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<0>>>
>);
static_assert(std::is_same_v<
	decltype("hex\"00\""_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<0>>>
>);
static_assert(std::is_same_v<
	decltype("hex'01'"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<1>>>
>);
static_assert(std::is_same_v<
	decltype("hex'10'"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<16>>>
>);
static_assert(std::is_same_v<
	decltype("hex'FF'"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<255>>>
>);
static_assert(std::is_same_v<
	decltype("hex'ff'"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<255>>>
>);
static_assert(std::is_same_v<
	decltype("hex'FF00'"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<255, 0>>>
>);
static_assert(std::is_same_v<
	decltype("hex'FF00112233'"_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<0xFF, 0x00, 0x11, 0x22, 0x33>>>
>);
static_assert(std::is_same_v<
	decltype("hex\"FF00112233\""_yulskers_expression),
	ast::Expression<ast::Literal<token::HexString<0xFF, 0x00, 0x11, 0x22, 0x33>>>
>);

// Strings
static_assert(std::is_same_v<
    decltype("'text'"_yulskers_expression),
    ast::Expression<ast::Literal<token::String<'t','e','x','t'>>>>
);
static_assert(std::is_same_v<
	decltype("\"text\""_yulskers_expression),
	ast::Expression<ast::Literal<token::String<'t','e','x','t'>>>>
);

// Placeholders.

static_assert(std::is_same_v<
	decltype("@x"_yulskers_expression),
	ast::Expression<ast::Placeholder<'x'>>
>);
static_assert(std::is_same_v<
	decltype("@xyz"_yulskers_expression),
	ast::Expression<ast::Placeholder<'x','y','z'>>
>);
static_assert(std::is_same_v<
	decltype("@complex.identifier$name_42"_yulskers_expression),
	ast::Expression<ast::Placeholder<
		'c', 'o', 'm', 'p', 'l', 'e', 'x', '.', 'i', 'd', 'e', 'n', 't', 'i', 'f', 'i', 'e', 'r',
		'$', 'n', 'a', 'm', 'e', '_', '4', '2'
	>>
>);
static_assert(std::is_same_v<
	decltype("@complex.identifier$name_42"_yulskers_expression),
	ast::Expression<decltype("complex.identifier$name_42"_yulskers_placeholder)>
>);

bool expressions_checked () { return true; }
