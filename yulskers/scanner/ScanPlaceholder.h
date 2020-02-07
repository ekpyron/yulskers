#pragma once

#include <yulskers/scanner/ScannerBase.h>
#include <yulskers/scanner/ScanIdentifier.h>

namespace yulskers::scanner {

template<typename Placeholder>
struct MakePlaceholder;

template<char... cs>
struct MakePlaceholder<char_list<cs...>>
{
	using type = token::Placeholder<cs...>;
};

template<typename State, typename CharList, typename Identifier = char_list<>, typename = void>
struct ScanPlaceholder:
	Scanner<add_token_t<State, typename MakePlaceholder<Identifier>::type>, CharList> {};

template<typename State, char c, char... cs, typename Identifier>
struct ScanPlaceholder<State, char_list<c, cs...>, Identifier, std::enable_if_t<isIdentifierPart(c)>>:
	ScanPlaceholder<advance_t<State>, char_list<cs...>, char_list_add_t<Identifier, c>> {};

}
