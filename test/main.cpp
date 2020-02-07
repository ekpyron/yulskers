#include <iostream>
#include <yulskers/yulskers.h>
#include <yulskers/ASTPrinter.h>

using namespace yulskers::literals;

int main()
{
	using AST = decltype(R"(
		{
			for { let x := 1 } lt(x,2) { x := add(x, 1) } {
				mstore(mul(x,42), 23)
				break
			}
			@stmt
			@fn(@arg)
			@var := @fn(@arg)
			@var, @var := @exp
			function f(x) -> a, b { a, b := g(x) return(a,b) }
			if mload(4) { sstore(4, 0) }
			let a := b(c, x(), z(a,b,c))
			let b.2$_1 := 0x12345A
			switch @test
			case 3 { x := 4 }
			case @lit { x := 4 }
			default { y := t }
			mstore(4, mload(hex"01AB0001"))
			sstore(4, "test")
			let A, B, C
			{
				continue
				break
				leave
			}
			let x := z
			let y := z
			let x := y
			let y := d
			let x := b
			let a := @b
			let b := c
			let a := f(@test)
			let c := @f(a, b, c)
			let X := Y
			a, b, f := f(f())
			test(a, b, f())
			continue
			continue
		}
	)"_yulskers);

	static_assert(!yulskers::is_failure_v<AST>, "");
	yulskers::PrintAST<AST, 0>{}(std::cout);

	return 0;
}
