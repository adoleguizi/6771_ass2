#include "./filtered_string_view.h"

#include <catch2/catch.hpp>

TEST_CASE("filter me if you can") {
	// for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); c++) {
	// 	std::cout << fsv::filtered_string_view::default_predicate(c);
	// }
	auto sv1 = fsv::filtered_string_view{"bulldog"};
	const auto copy = sv1;

	CHECK(copy.data() == sv1.data()); // pointers compare equal.

	const auto move = std::move(sv1);
	CHECK(sv1.data() == nullptr); // true: sv1's guts were moved into `move`

	// REQUIRE(false);
}
TEST_CASE("Default Constructor") {
	auto sv = fsv::filtered_string_view{};
	std::cout << sv.size() << std::endl;
	CHECK(sv.size() == 0);
}
TEST_CASE("Implicit String Constructor") {
	auto s = std::string{"cat"};
	auto sv = fsv::filtered_string_view{s};
	std::cout << sv.size() << std::endl;
	CHECK(sv.size() == 3);
}
TEST_CASE("Implicit String Constructor empty string") {
	auto s = std::string{""};
	auto sv = fsv::filtered_string_view{s};
	std::cout << sv.size() << std::endl;
	CHECK(sv.size() == 0);
}
TEST_CASE("String Constructor with Predicate") {
	auto s = std::string{"cat"};
	auto pred = [](const char& c) { return c == 'a'; };
	auto sv = fsv::filtered_string_view{s, pred};
	std::cout << sv.size() << std::endl;
	CHECK(sv.size() == 1);
}
TEST_CASE("String Constructor with No Matching Characters", "[FilteredStringView]") {
	auto s = std::string{"xyz"};
	auto pred = [](const char& c) { return c == 'a'; }; // Filter that does not match any characters in "xyz"
	auto sv = fsv::filtered_string_view{s, pred};

	std::cout << sv.size() << std::endl; // Expected output: 0
	CHECK(sv.size() == 0); // No 'a' in "xyz"

	// Verify that trying to access any index throws an exception
	REQUIRE_THROWS_AS(sv.at(0), std::domain_error);
}

TEST_CASE(" Implicit Null-Terminated String Constructor") {
	auto sv = fsv::filtered_string_view{"cat"};
	std::cout << sv.size() << std::endl;
	CHECK(sv.size() == 3);
}
TEST_CASE("  Null-Terminated String with Predicate Constructor") {
	auto pred = [](const char& c) { return c == 'a'; };
	auto sv = fsv::filtered_string_view{"cat", pred};
	std::cout << sv.size();
	CHECK(sv.size() == 1);
}
TEST_CASE("Copy and Move Constructors") {
	auto sv1 = fsv::filtered_string_view{"bulldog"};
	const auto copy = sv1;

	CHECK(copy.data() == sv1.data()); // pointers compare equal.

	const auto move = std::move(sv1);
	CHECK(sv1.data() == nullptr); // true: sv1's guts were moved into `move`
}
// TEST_CASE("Copy Assignment") {
// 	auto pred = [](const char &c) { return c == '4' || c == '2'; };
// 	auto fsv1 = fsv::filtered_string_view{"42 bro", pred};
// 	auto fsv2 = fsv::filtered_string_view{};
// 	fsv2 = fsv1;
// 	assert(fsv1 == fsv2);
// }
TEST_CASE("Move Assignment") {
	auto pred = [](const char& c) { return c == '8' || c == '9'; };
	auto fsv1 = fsv::filtered_string_view{"'89 baby", pred};
	std::cout << fsv1.size() << std::endl;
	auto fsv2 = fsv::filtered_string_view{};

	fsv2 = std::move(fsv1);

	CHECK(fsv1.size() == 0);
	CHECK(fsv1.data() == nullptr);
}

TEST_CASE("Subscript") {
	auto pred = [](const char& c) { return c == '9' || c == '0' || c == ' '; };
	auto fsv1 = fsv::filtered_string_view{"only 90s kids understand", pred};
	std::cout << fsv1[2] << std::endl;
	CHECK(fsv1[2] == '0');
}
TEST_CASE("String Type Conversion Basic") {
	auto sv = fsv::filtered_string_view("vizsla");
	auto s = static_cast<std::string>(sv);
	std::cout << std::boolalpha << (sv.data() == s.data()) << std::endl;
}
TEST_CASE("String Type Conversion Memory and Immutability") {
	fsv::filtered_string_view sv{"vizsla"};
	auto s = static_cast<std::string>(sv); // Convert to std::string

	// Check if the converted string matches the expected value
	REQUIRE(s == "vizsla");

	// Check that the memory addresses are different (i.e., it is a deep copy)
	REQUIRE_FALSE(sv.data() == s.data());

	// Modify the std::string and ensure the filtered_string_view is unaffected
	s[0] = 'V'; // Change 'v' to 'V' in the std::string
	REQUIRE(sv.at(0) == 'v'); // Ensure the original view is unchanged
}
TEST_CASE("at in vowel with predicate") {
	auto vowels = std::set<char>{'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
	auto is_vowel = [&vowels](const char& c) { return vowels.contains(c); };
	auto sv = fsv::filtered_string_view{"Malamute", is_vowel};
	// CHECK(sv =='a') after implementing == operator
}
TEST_CASE("AccessEmptyString", "[FilteredStringView]") {
	fsv::filtered_string_view sv{""}; // Create an empty filtered_string_view.
	// Verify that calling at(0) throws a std::domain_error, as expected.
	REQUIRE_THROWS_AS(sv.at(0), std::domain_error);
}

TEST_CASE("AccessBeyondUpperBound", "[FilteredStringView]") {
	auto pred = fsv::filtered_string_view::default_predicate; // Predicate that accepts all characters
	fsv::filtered_string_view sv{"hello", pred}; // Create a filtered_string_view with some content.
	// Verify that accessing beyond the string's length throws a std::domain_error
	REQUIRE_THROWS_AS(sv.at(5), std::domain_error); // "hello" length is 5, index 5 is out of bounds
}
TEST_CASE("AccessNegativeIndex", "[FilteredStringView]") {
	auto pred = fsv::filtered_string_view::default_predicate; // Predicate that accepts all characters
	fsv::filtered_string_view sv{"hello", pred}; // Create a filtered_string_view with some content.
	// Verify that using a negative index throws a std::domain_error
	REQUIRE_THROWS_AS(sv.at(-1), std::domain_error); // Negative index is always out of bounds
}
TEST_CASE("AccessEmptyStringWithPredicate", "[FilteredStringView]") {
	auto pred = [](const char& c) { return c == 'a'; }; // Predicate that accepts 'a'
	fsv::filtered_string_view sv{"banana", pred}; // Create a filtered_string_view.
	// Verify that accessing index that is valid for the original string but not for filtered results
	REQUIRE_THROWS_AS(sv.at(3), std::domain_error); // "banana" filtered has "aaa", index 3 is out of bounds
}
TEST_CASE("size of filtered_string_view") {
	auto sv = fsv::filtered_string_view{"Maltese"};
	std::cout << sv.size();
	CHECK(sv.size() == 7);
}
TEST_CASE("size of filtered_string_view with predicate") {
	auto sv = fsv::filtered_string_view{"Toy Poodle", [](const char& c) { return c == 'o'; }};
	std::cout << sv.size();
	CHECK(sv.size() == 3);
}
TEST_CASE("size of filtered_string_view with empty string") {
	auto sv = fsv::filtered_string_view{""};
	CHECK(sv.size() == 0);
}
TEST_CASE("size of filtered_string_view with all matching predicate", "[FilteredStringView]") {
	auto sv = fsv::filtered_string_view{"aaaaaa", [](const char& c) { return c == 'a'; }};
	CHECK(sv.size() == 6);
}
TEST_CASE("size of filtered_string_view with no matching predicate", "[FilteredStringView]") {
	auto sv = fsv::filtered_string_view{"abcde", [](const char& c) { return c == 'z'; }};
	CHECK(sv.size() == 0);
}
TEST_CASE("size of filtered_string_view with complex predicate") {
	auto sv = fsv::filtered_string_view{"ab1c2d3e4", [](const char& c) { return isdigit(c); }};
	CHECK(sv.size() == 4); // Only the digits should be counted
}
TEST_CASE("empty of filtered_string_view with empty and non-empty string") {
	auto sv = fsv::filtered_string_view{"Australian Shephard"};
	auto empty_sv = fsv::filtered_string_view{};
	std::cout << std::boolalpha << sv.empty() << ' ' << empty_sv.empty();
	CHECK_FALSE(sv.empty());
	CHECK(empty_sv.empty());
}
TEST_CASE("empty of filtered_string_view with predicate") {
	auto sv = fsv::filtered_string_view{"Border Collie", [](const char& c) { return c == 'z'; }};
	std::cout << std::boolalpha << sv.empty();
	CHECK(sv.empty());
}
