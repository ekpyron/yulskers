#pragma once

#include <yulskers/AST.h>
#include <ostream>
#include <iomanip>

namespace yulskers {

template<int indent>
static const auto indentation = std::string(indent, '\t');

template<typename Block, int indent = 0, bool Inline = false>
struct PrintBlock;

template<template<typename> typename Printer, typename... Arguments>
struct PrintCommaSeparatedList;
template<template<typename> typename Printer>
struct PrintCommaSeparatedList<Printer>
{
	void operator()(std::ostream& _stream) const
	{
	}
};
template<template<typename> typename Printer, typename Argument, typename... Arguments>
struct PrintCommaSeparatedList<Printer, Argument, Arguments...>
{
	void operator()(std::ostream& _stream) const
	{
		Printer<Argument>{}(_stream);
		(Printer<Arguments>{}(_stream << ", "), ...);
	}
};


template<typename Identifier>
struct PrintPlaceholder;
template<char... cs>
struct PrintPlaceholder<ast::Placeholder<cs...>>
{
	void operator()(std::ostream& _stream) const
	{
		static const char name[sizeof...(cs)] = { cs... };
		_stream << "@" << std::string(name, name + sizeof...(cs));
	}
};

template<typename Identifier>
struct PrintIdentifier;
template<char... cs>
struct PrintIdentifier<ast::Identifier<cs...>>
{
	void operator()(std::ostream& _stream) const
	{
		static const char name[sizeof...(cs)] = { cs... };
		_stream << std::string(name, name + sizeof...(cs));
	}
};
template<char... cs>
struct PrintIdentifier<ast::Placeholder<cs...>>
{
	void operator()(std::ostream& _stream) const
	{
		static const char name[sizeof...(cs)] = { cs... };
		_stream << "@" << std::string(name, name + sizeof...(cs));
	}
};

template<typename Kind>
struct PrintLiteral;

template<char... cs>
struct PrintLiteral<ast::Literal<ast::Placeholder<cs...>>>
{
	void operator()(std::ostream& _stream) const
	{
		PrintPlaceholder<ast::Placeholder<cs...>>{}(_stream);
	}
};

template<char... vs>
struct PrintLiteral<ast::Literal<token::DecimalNumber<vs...>>>
{
	void operator()(std::ostream& _stream) const
	{
		((_stream << std::dec << int(vs)), ...);
	}
};

template<std::uint8_t... vs>
struct PrintLiteral<ast::Literal<token::HexString<vs...>>>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << "hex\"";
		((_stream << std::hex << std::setw(2) << std::setfill('0') << int(vs)), ...);
		_stream << "\"";
	}
};

template<char... cs>
struct PrintLiteral<ast::Literal<token::String<cs...>>>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << "\"";
		((_stream << cs), ...);
		_stream << "\"";
	}
};

template<char... vs>
struct PrintLiteral<ast::Literal<token::HexNumber<vs...>>>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << "0x";
		((_stream << std::hex << int(vs)), ...);
	}
};

template<typename Expression>
struct PrintExpression;

template<char... cs>
struct PrintExpression<ast::Expression<ast::Identifier<cs...>>>
{
	void operator()(std::ostream& _stream) const
	{
		PrintIdentifier<ast::Identifier<cs...>>{}(_stream);
	}
};

template<char... cs>
struct PrintExpression<ast::Expression<ast::Placeholder<cs...>>>
{
	void operator()(std::ostream& _stream) const
	{
		PrintIdentifier<ast::Placeholder<cs...>>{}(_stream);
	}
};

template<typename Kind>
struct PrintExpression<ast::Expression<ast::Literal<Kind>>>
{
	void operator()(std::ostream& _stream) const
	{
		PrintLiteral<ast::Literal<Kind>>{}(_stream);
	}
};

template<typename Name, typename... Arguments>
struct PrintExpression<ast::Expression<ast::FunctionCall<Name, type_list<Arguments...>>>>
{
	void operator()(std::ostream& _stream) const
	{
		PrintIdentifier<Name>{}(_stream);
		_stream << "(";
		PrintCommaSeparatedList<PrintExpression, Arguments...>{}(_stream);
		_stream << ")";
	}
};


template<typename Statement, int indent>
struct PrintStatement;

template<typename Init, typename Condition, typename Post, typename Body, int indent>
struct PrintStatement<ast::For<Init, Condition, Post, Body>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "for ";
		PrintBlock<Init, indent, true>{}(_stream);
		_stream << " ";
		PrintExpression<Condition>{}(_stream);
		_stream << " ";
		PrintBlock<Post, indent, true>{}(_stream);
		_stream << " ";
		PrintBlock<Body, indent, true>{}(_stream);
		_stream << std::endl;
	}
};

template<char... cs, int indent>
struct PrintStatement<ast::Placeholder<cs...>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent>;
		PrintPlaceholder<ast::Placeholder<cs...>>{}(_stream);
		_stream << std::endl;
	}
};

template<typename Condition, typename Body, int indent>
struct PrintStatement<ast::If<Condition, Body>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "if ";
		PrintExpression<Condition>{}(_stream);
		_stream << " ";
		PrintBlock<Body, indent, true>{}(_stream);
		_stream << std::endl;
	}
};

template<typename Name, typename... Arguments, typename... Returns, typename Body, int indent>
struct PrintStatement<ast::Function<Name, type_list<Arguments...>, type_list<Returns...>, Body>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "function ";
		PrintIdentifier<Name>{}(_stream);
		_stream << "(";
		PrintCommaSeparatedList<PrintIdentifier, Arguments...>{}(_stream);
		_stream << ")";
		if constexpr (sizeof...(Returns) > 0)
		{
			_stream << " => ";
			PrintCommaSeparatedList<PrintIdentifier, Returns...>{}(_stream);
		}
		_stream << " ";
		PrintBlock<Body, indent, true>{}(_stream);
		_stream << std::endl;
	}
};

template<typename Case, int indent>
struct PrintSwitchCase;

template<typename Block, int indent>
struct PrintSwitchCase<type_list<Block>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "default ";
		PrintBlock<Block, indent, true>{}(_stream);
		_stream << std::endl;
	}
};

template<typename Literal, typename Block, int indent>
struct PrintSwitchCase<type_list<Literal, Block>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "case ";
		PrintLiteral<Literal>{}(_stream);
		_stream << " ";
		PrintBlock<Block, indent, true>{}(_stream);
		_stream << std::endl;
	}
};

template<typename Expression, typename... Cases, int indent>
struct PrintStatement<ast::Switch<Expression, Cases...>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "switch ";
		PrintExpression<Expression>{}(_stream);
		_stream << std::endl;
		(PrintSwitchCase<Cases, indent>{}(_stream), ...);
	}
};

template<typename Name, typename... Arguments, int indent>
struct PrintStatement<ast::FunctionCall<Name, type_list<Arguments...>>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation <indent>;
		PrintExpression<ast::Expression<ast::FunctionCall<Name, type_list<Arguments...>>>>{}(_stream);
		_stream << std::endl;
	}
};

template<typename... Variables, typename Value, int indent>
struct PrintStatement<ast::Assignment<type_list<Variables...>, Value>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation <indent>;
		PrintCommaSeparatedList<PrintIdentifier, Variables...>{}(_stream);
		_stream << " := ";
		PrintExpression<Value>{}(_stream);
		_stream << std::endl;
	}
};

template<typename... Variables, typename Value, int indent>
struct PrintStatement<ast::VariableDeclaration<type_list<Variables...>, Value>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation <indent> << "let ";
		PrintCommaSeparatedList<PrintIdentifier, Variables...>{}(_stream);
		if constexpr (!std::is_same_v<Value, void>) {
			_stream << " := ";
			PrintExpression<Value>{}(_stream);
		}
		_stream << std::endl;
	}
};

template<typename... Statements, int indent>
struct PrintStatement<ast::Block<Statements...>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		PrintBlock<ast::Block<Statements...>, indent>{}(_stream);
	}
};

template<int indent>
struct PrintStatement<ast::Continue, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "continue" << std::endl;
	}
};

template<int indent>
struct PrintStatement<ast::Break, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "break" << std::endl;
	}
};

template<int indent>
struct PrintStatement<ast::Leave, indent>
{
	void operator()(std::ostream& _stream) const
	{
		_stream << indentation<indent> << "leave" << std::endl;
	}
};

template<typename... Statements, int indent, bool Inline>
struct PrintBlock<ast::Block<Statements...>, indent, Inline>
{
	void operator()(std::ostream& _stream) const
	{
		if constexpr (!Inline)
			_stream << indentation<indent>;
		_stream << "{";
		_stream << std::endl;
		(PrintStatement<Statements, indent + 1>{}(_stream), ...);
		_stream << indentation<indent> << "}";
		if constexpr (!Inline)
			_stream << std::endl;
	}
};

template<typename AST, int indent = 0>
struct PrintAST;

template<typename... Blocks, int indent>
struct PrintAST<type_list<Blocks...>, indent>
{
	void operator()(std::ostream& _stream) const
	{
		(PrintBlock<Blocks, indent>{}(_stream), ...);
	}
};

}