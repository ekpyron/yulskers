#include <yulskers/yulskers.h>

using namespace yulskers;
using namespace yulskers::literals;

/// ast::Block
static_assert(std::is_same_v<
	decltype("{}"_yulskers_block),
	ast::Block<>
>);
static_assert(std::is_same_v<
	decltype("{{}}"_yulskers_block),
	ast::Block<ast::Block<>>
			  >);
static_assert(std::is_same_v<
	decltype("{ { } { } }"_yulskers_block),
	ast::Block<ast::Block<>, ast::Block<>>
			  >);
static_assert(std::is_same_v<
	decltype("{ { {} } { } }"_yulskers_block),
	ast::Block<ast::Block<ast::Block<>>, ast::Block<>>
>);

bool blocks_checked() { return true; }
