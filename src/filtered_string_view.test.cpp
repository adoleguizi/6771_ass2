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
// TEST_CASE("Copy Assignment") {
// 	auto pred = [](const char &c) { return c == '4' || c == '2'; };
// 	auto fsv1 = fsv::filtered_string_view{"42 bro", pred};
// 	auto fsv2 = fsv::filtered_string_view{};
// 	fsv2 = fsv1;
// 	assert(fsv1 == fsv2);
//
//
// 	// REQUIRE(false);
// }
TEST_CASE(" Implicit Null-Terminated String Constructor") {
	auto sv = fsv::filtered_string_view{"cat"};
	std::cout << sv.size() << std::endl;
	assert(sv.size() == 3);
}
TEST_CASE("  Null-Terminated String with Predicate Constructor") {
	auto pred = [](const char& c) { return c == 'a'; };
	auto sv = fsv::filtered_string_view{"cat", pred};
	std::cout << sv.size();
	assert(sv.size() == 1);
}
TEST_CASE("Copy and Move Constructors") {
	auto sv1 = fsv::filtered_string_view{"bulldog"};
	const auto copy = sv1;

	assert(copy.data() == sv1.data()); // pointers compare equal.

	const auto move = std::move(sv1);
	assert(sv1.data() == nullptr); // true: sv1's guts were moved into `move`
}