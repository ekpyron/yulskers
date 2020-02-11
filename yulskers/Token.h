#pragma once

#include <type_traits>

namespace yulskers {

enum class TokenKind
{
	WhiteSpace,
	String,
	Identifier,
	Placeholder,
	Let,
	If,
	Switch,
	Case,
	Default,
	For,
	Break,
	Continue,
	Function,
	Leave,
	HexNumber,
	DecimalNumber,
	BracketOpen,
	BracketClose,
	ParenOpen,
	ParenClose,
	Assign,
	Arrow,
	Comma,
	Colon,
	True,
	False,
	Comment,
	HexString
};

namespace token {
//struct WhiteSpace { static constexpr auto kind = TokenKind::WhiteSpace; };
template<char... c>
struct String { static constexpr auto kind = TokenKind::String; };
template<char... c>
struct Identifier { static constexpr auto kind = TokenKind::Identifier; };
template<char... c>
struct Placeholder { static constexpr auto kind = TokenKind::Placeholder; };
struct Let { static constexpr auto kind = TokenKind::Let; };
struct If { static constexpr auto kind = TokenKind::If; };
struct Switch { static constexpr auto kind = TokenKind::Switch; };
struct Case { static constexpr auto kind = TokenKind::Case; };
struct Default { static constexpr auto kind = TokenKind::Default; };
struct For { static constexpr auto kind = TokenKind::For; };
struct Break { static constexpr auto kind = TokenKind::Break; };
struct Continue { static constexpr auto kind = TokenKind::Continue; };
struct Function { static constexpr auto kind = TokenKind::Function; };
struct Leave { static constexpr auto kind = TokenKind::Leave; };
template<char...>
struct HexNumber { static constexpr auto kind = TokenKind::HexNumber; };
template<char...>
struct DecimalNumber { static constexpr auto kind = TokenKind::DecimalNumber; };
struct BracketOpen { static constexpr auto kind = TokenKind::BracketOpen; };
struct BracketClose { static constexpr auto kind = TokenKind::BracketClose; };
struct ParenOpen { static constexpr auto kind = TokenKind::ParenOpen; };
struct ParenClose { static constexpr auto kind = TokenKind::ParenClose; };
struct Assign { static constexpr auto kind = TokenKind::Assign; };
struct Arrow { static constexpr auto kind = TokenKind::Arrow; };
struct Comma { static constexpr auto kind = TokenKind::Comma; };
struct Colon { static constexpr auto kind = TokenKind::Colon; };
struct True { static constexpr auto kind = TokenKind::True; };
struct False { static constexpr auto kind = TokenKind::False; };
//template<bool multeLine, char... cs>
//struct Comment { static constexpr auto kind = TokenKind::Comment; };
template<std::uint8_t...>
struct HexString { static constexpr auto kind = TokenKind::HexString; };
}

namespace token_traits
{
	template<typename Token>
	using token_type_t = Token;
	template<typename Token>
	struct token_kind { static constexpr auto value = Token::kind; };
	template<typename Token>
	constexpr auto token_kind_v = Token::kind;
	template<typename Token>
	constexpr bool is_literal_v = token_traits::token_kind_v<Token> == TokenKind::HexNumber ||
								  token_traits::token_kind_v<Token> == TokenKind::DecimalNumber ||
								  token_traits::token_kind_v<Token> == TokenKind::String ||
								  token_traits::token_kind_v<Token> == TokenKind::HexString;
}

}
