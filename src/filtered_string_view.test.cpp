#include "./filtered_string_view.h"

#include <catch2/catch.hpp>

TEST_CASE("filter me if you can") {
	// for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); c++) {
	// 	std::cout << fsv::filtered_string_view::default_predicate(c);
	// }
	auto sv1 = fsv::filtered_string_view{"bulldog"};
	const auto copy = sv1;

	assert(copy.data() == sv1.data()); // pointers compare equal.

	const auto move = std::move(sv1);
	assert(sv1.data() == nullptr); // true: sv1's guts were moved into `move`

	// REQUIRE(false);
}
