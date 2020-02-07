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
struct WhiteSpace { static constexpr auto kind = TokenKind::WhiteSpace; };
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
template<bool multeLine, char... cs>
struct Comment { static constexpr auto kind = TokenKind::Comment; };
template<std::uint8_t...>
struct HexString { static constexpr auto kind = TokenKind::HexString; };
}

template<typename TokenType, std::size_t Start, std::size_t End>
struct Token
{
	using type = TokenType;
	static constexpr std::size_t start = Start;
	static constexpr std::size_t end = End;
};

namespace token_traits
{
	template<typename Token, typename = void>
	struct is_whitespace: std::false_type {};
	template<typename Token>
	struct is_whitespace<Token, std::enable_if_t<std::is_same_v<typename Token::type, token::WhiteSpace>>>: std::true_type {};
	template<typename Token>
	static constexpr bool is_whitespace_v = is_whitespace<Token>::value;

	template<typename Token, typename = void>
	struct is_comment: std::false_type {};
	template<bool a, char... cs, std::size_t d, std::size_t e>
	struct is_comment<Token<token::Comment<a, cs...>, d, e>>: std::true_type {};
	template<typename Token>
	static constexpr bool is_comment_v = is_comment<Token>::value;

	template<typename Token>
	struct token_type { using type = typename Token::type; };
	template<typename Token>
	struct token_kind { static constexpr auto value = Token::type::kind; };
	template<typename Token>
	constexpr auto token_kind_v = token_kind<Token>::value;

	template<typename Token>
	constexpr bool is_literal_v = token_traits::token_kind_v<Token> == TokenKind::HexNumber ||
								  token_traits::token_kind_v<Token> == TokenKind::DecimalNumber ||
								  token_traits::token_kind_v<Token> == TokenKind::String ||
								  token_traits::token_kind_v<Token> == TokenKind::HexString;

}

template<typename TokenList>
struct drop_whitespace_tokens: type_list_filter<TokenList, token_traits::is_whitespace, false> {};
template<typename TokenList>
using drop_whitespace_tokens_t = typename drop_whitespace_tokens<TokenList>::type;

template<typename TokenList>
struct drop_comment_tokens: type_list_filter<TokenList, token_traits::is_comment, false> {};
template<typename TokenList>
using drop_comment_tokens_t = typename drop_comment_tokens<TokenList>::type;

template<typename TokenList>
struct drop_locations: type_list_map<TokenList, token_traits::token_type> {};
template<typename TokenList>
using drop_locations_t = typename drop_locations<TokenList>::type;

}
