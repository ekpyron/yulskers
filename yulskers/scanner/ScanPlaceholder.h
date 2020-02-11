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

template<typename CharList, typename Identifier = char_list<>, typename = void>
struct ScanPlaceholder
{
	using type = typename MakePlaceholder<Identifier>::type;
	using next = Scanner<CharList>;
};

template<char c, char... cs, typename Identifier>
struct ScanPlaceholder<char_list<c, cs...>, Identifier, std::enable_if_t<isIdentifierPart(c)>>:
	ScanPlaceholder<char_list<cs...>, char_list_add_t<Identifier, c>> {};

}
