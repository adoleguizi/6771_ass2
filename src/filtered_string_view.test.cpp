#include "./filtered_string_view.h"

#include <catch2/catch.hpp>

TEST_CASE("filter me if you can") {
	for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); c++) {
		std::cout << fsv::filtered_string_view::default_predicate(c);
	}

	auto pred = [](const char& c) { return c == 'a'; };
	auto sv = fsv::filtered_string_view{"cat", pred};
	std::cout << sv.size() << std::endl;
	// REQUIRE(false);
}
