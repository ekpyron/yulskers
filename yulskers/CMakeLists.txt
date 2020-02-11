set(YULSKERS_HEADERS
        yulskers.h
        AST.h
        common.h
        common/op_chain.h
        common/type_list.h
        Token.h
        parser/ParseBlock.h
        parser/ParseExpression.h
        parser/ParseIdentifier.h
        parser/Parser.h
        parser/ParserBase.h
        parser/ParserState.h
        parser/ParseStatement.h
        scanner/ScanComment.h
        scanner/ScanHexString.h
        scanner/ScanIdentifier.h
        scanner/Scanner.h
        scanner/ScannerBase.h
        scanner/ScannerState.h
        scanner/ScanNumber.h
        scanner/ScanString.h
        scanner/ScanWhitespace.h
)

add_library(yulskers INTERFACE)

target_sources(yulskers INTERFACE ${YULSKERS_HEADERS})

target_include_directories(yulskers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(yulskers INTERFACE -Wno-gnu-string-literal-operator-template)
endif ()