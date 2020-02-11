#pragma once

namespace yulskers {

namespace scanner {
template<typename CharList, typename = void>
struct Scanner;
}

namespace parser {
template<typename Scanner, typename AST>
struct ParserState;
}

}
