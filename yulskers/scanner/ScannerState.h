#pragma once

#include <yulskers/common.h>
#include <yulskers/Token.h>

namespace yulskers::scanner {

template<typename TokenList, std::size_t tokenStartPos, std::size_t currentPos >
struct ScannerState
{
	using tokens = TokenList;
	static constexpr bool placeholders = true;
};
using EmptyScannerState = ScannerState<type_list<>, 0, 0>;

template<typename State, typename TokenType>
struct add_token;
template<typename State, typename TokenType>
using add_token_t = typename add_token<State, TokenType>::type;

template<typename... Tokens, std::size_t tokenStartPos, std::size_t currentPos, typename TokenType>
struct add_token<ScannerState<type_list<Tokens...>, tokenStartPos, currentPos>, TokenType>
{
	using type = ScannerState<type_list<Tokens..., Token<TokenType, tokenStartPos, currentPos>>, currentPos, currentPos>;
};

template<typename State, std::size_t amount = 1>
struct advance;
template<typename State, std::size_t amount = 1>
using advance_t = typename advance<State, amount>::type;

template<typename TokenList, std::size_t tokenStartPos, std::size_t currentPos, std::size_t amount>
struct advance<ScannerState<TokenList, tokenStartPos, currentPos>, amount>
{
	using type = ScannerState<TokenList, tokenStartPos, currentPos + amount>;
};

}
