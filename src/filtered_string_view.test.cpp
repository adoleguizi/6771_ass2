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
TEST_CASE("empty with all characters matching predicate") {
	auto sv = fsv::filtered_string_view{"Ccccc", [](const char& c) { return c == 'C' || c == 'c'; }};
	CHECK_FALSE(sv.empty());
}
TEST_CASE("empty with single match predicate") {
	auto sv = fsv::filtered_string_view{"Labrador", [](const char& c) { return c == 'a'; }};
	CHECK_FALSE(sv.empty());
}
TEST_CASE("filtered_string_view with false predicate", "[FilteredStringView]") {
	const char* s = "Sum 42";
	fsv::filtered_string_view sv{s, [](const char& c) {
		                             (void)c;
		                             return false;
	                             }};

	// Check if size is correctly reporting zero
	CHECK(sv.size() == 0);

	// Check if the data() still returns the original string despite the predicate
	const char* ptr = sv.data();
	std::string output;
	for (; *ptr; ++ptr) {
		output.push_back(*ptr);
	}

	CHECK(output == "Sum 42"); // Ensure the output matches the original string
}
TEST_CASE("Predicate accessor usage with output check", "[FilteredStringView]") {
	const auto print_and_return_true = [](const char&) {
		std::cout << "hi!";
		return true;
	};
	const auto s = fsv::filtered_string_view{"doggo", print_and_return_true};

	std::stringstream buffer;

	auto* coutbuf = std::cout.rdbuf();

	std::cout.rdbuf(buffer.rdbuf());

	const auto& pred = s.predicate();
	pred('c'); // Call the predicate
	std::cout.rdbuf(coutbuf);
	// check if the output is correct
	CHECK(buffer.str() == "hi!");
	// validate the return value of the predicate
	CHECK(pred('x') == true);
}

TEST_CASE("Equality comparison") {
	auto const lo = fsv::filtered_string_view{"aaa"};
	auto const hi = fsv::filtered_string_view{"zzz"};

	std::cout << std::boolalpha << (lo == hi) << ' ' << (lo != hi);
	CHECK_FALSE(lo == hi);
	CHECK(lo != hi);
}
// TEST_CASE("Equality comparison ignores predicates") {
// 	const char* text1 = "example";
// 	const char* text2 = "example";
// 	auto predicate1 = [](char c) { return c == 'a'; };
// 	auto predicate2 = [](char c) { return c == 'e'; };
//
// 	auto const view1 = fsv::filtered_string_view{text1, predicate1};
// 	auto const view2 = fsv::filtered_string_view{text2, predicate2};
//
// 	// Check if the views are equal despite having different predicates
// 	CHECK(view1 == view2);
// 	CHECK_FALSE(view1 != view2);
// }
TEST_CASE("Equality comparison with identical views") {
	auto const view1 = fsv::filtered_string_view{"same"};
	auto const view2 = fsv::filtered_string_view{"same"};

	CHECK(view1 == view2);
	CHECK_FALSE(view1 != view2);
}
TEST_CASE("Equality comparison of empty views") {
	auto const empty1 = fsv::filtered_string_view{""};
	auto const empty2 = fsv::filtered_string_view{""};

	CHECK(empty1 == empty2);
	CHECK_FALSE(empty1 != empty2);
}
TEST_CASE("Equality comparison of different lengths") {
	auto const shorter = fsv::filtered_string_view{"short"};
	auto const longer = fsv::filtered_string_view{"shorter"};

	CHECK_FALSE(shorter == longer);
	CHECK(shorter != longer);
}
TEST_CASE("Equality comparison of same length but different content") {
	auto const alpha = fsv::filtered_string_view{"alpha"};
	auto const beta = fsv::filtered_string_view{"betas"};

	CHECK_FALSE(alpha == beta);
	CHECK(alpha != beta);
}
TEST_CASE("Predicate-based comparisons", "[filtered_string_view]") {
	std::function<bool(char)> is_lower = [](char c) { return std::islower(c); };
	auto const lower = fsv::filtered_string_view{"abcdABCD", is_lower};
	auto const upper = fsv::filtered_string_view{"ABCDabcd", is_lower};
	// 输出转换后的字符串以确认过滤效果
	std::string lower_str = lower.operator std::string();
	std::string upper_str = upper.operator std::string();
	std::cout << "Lower filtered: " << lower_str << std::endl;
	std::cout << "Upper filtered: " << upper_str << std::endl;

	// 检查过滤后的字符串是否如预期
	CHECK(lower_str == "abcd");
	CHECK(upper_str == "abcd");

	// 谓词过滤后实际上两个字符串都应视为 "abcd"
	CHECK((lower <=> upper) == std::strong_ordering::equal);
	CHECK((lower == upper) == true);
}
TEST_CASE("Spaceship operator and derived comparisons", "[filtered_string_view]") {
	auto const lo = fsv::filtered_string_view{"aaa"};
	auto const hi = fsv::filtered_string_view{"zzz"};
	auto const eq = fsv::filtered_string_view{"aaa"};

	// Test for less than, less than or equal, greater than, greater than or equal, and exact comparison
	CHECK((lo < hi) == true);
	CHECK((lo <= hi) == true);
	CHECK((hi > lo) == true);
	CHECK((hi >= lo) == true);
	CHECK((lo <=> hi) == std::strong_ordering::less);
	CHECK((lo <=> eq) == std::strong_ordering::equal);
	CHECK((hi <=> lo) == std::strong_ordering::greater);

	// Edge cases: comparison with itself
	CHECK((lo < lo) == false);
	CHECK((lo <= lo) == true);
	CHECK((lo > lo) == false);
	CHECK((lo >= lo) == true);
	CHECK((lo <=> lo) == std::strong_ordering::equal);
}
TEST_CASE("Filter only 'c' and '+' characters", "[filtered_string_view]") {
	auto fsv = fsv::filtered_string_view{"c++ > rust > java", [](const char& c) { return c == 'c' || c == '+'; }};
	std::ostringstream os;
	os << fsv;
	REQUIRE(os.str() == "c++"); // Confirm that only 'c' and '+' are output
}
TEST_CASE("Filter non-existing character leads to empty output", "[filtered_string_view]") {
	auto fsv = fsv::filtered_string_view{"c++ > rust > java", [](const char& c) {
		                                     return c == 'x'; // Non-existing character in the string
	                                     }};
	std::ostringstream os;
	os << fsv;
	REQUIRE(os.str() == ""); // Confirm that output is empty
}
TEST_CASE("Composite filters with compose function", "[filtered_string_view]") {
	auto best_languages = fsv::filtered_string_view{"c / c++"};
	// define a vector of filters
	auto vf = std::vector<fsv::filter>{
	    [](const char& c) { return c == 'c' || c == '+' || c == '/'; }, // 允许 'c', '+', '/'
	    [](const char& c) { return c > ' '; }, // filter empty space
	    [](const char&) { return true; } // no-condition filter
	};
	// using compose function to combine the filters
	auto sv = fsv::compose(best_languages, vf);
	// ostream output
	std::ostringstream os;
	os << sv;
	REQUIRE(os.str() == "c/c++"); // Confirm that only 'c', '+', '/' are output
}
TEST_CASE("Handling empty string with filters", "[filtered_string_view]") {
	auto empty_fsv = fsv::filtered_string_view{""};
	auto filters = std::vector<fsv::filter>{[](char c) {
		                                        (void)c;
		                                        return true;
	                                        },
	                                        [](char c) { return c == 'a'; }};

	auto composed_fsv = fsv::compose(empty_fsv, filters);
	std::ostringstream os;
	os << composed_fsv;
	REQUIRE(os.str().empty()); // 确认输出为空字符串
}
TEST_CASE("Compose with no filters", "[filtered_string_view]") {
	auto fsv = fsv::filtered_string_view{"Test string"};
	auto empty_filters = std::vector<fsv::filter>{};

	auto composed_fsv = fsv::compose(fsv, empty_filters);
	std::ostringstream os;
	os << composed_fsv;
	REQUIRE(os.str() == "Test string"); // 验证输出与输入相同
}
TEST_CASE("All filters reject", "[filtered_string_view]") {
	auto fsv = fsv::filtered_string_view{"c++ > rust > java"};
	auto reject_all = std::vector<fsv::filter>{[](char) { return false; }, [](char) { return false; }};

	auto composed_fsv = fsv::compose(fsv, reject_all);
	std::ostringstream os;
	os << composed_fsv;
	REQUIRE(os.str().empty()); // 确认输出为空
}
TEST_CASE("Split filtered_string_view on delimiter", "[split]") {
	// 定义字符集合
	std::set<char> interest = {'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', ' ', '/'};
	// 创建 filtered_string_view 实例
	fsv::filtered_string_view sv{"0xDEADBEEF / 0xdeadbeef", [&interest](const char& c) { return interest.contains(c); }};
	// 分隔符
	fsv::filtered_string_view tok{" / "};

	// 执行分割操作
	auto v = fsv::split(sv, tok);

	// 检查分割后的数量和内容
	REQUIRE(v.size() == 2); // 确保正确地分割为两部分
	CHECK(v[0] == fsv::filtered_string_view{"DEADBEEF"}); // 检查第一部分内容
	CHECK(v[1] == fsv::filtered_string_view{"deadbeef"}); // 检查第二部分内容
}
TEST_CASE("Split on single character with multiple occurrences", "[split]") {
	auto sv = fsv::filtered_string_view{"xx"};
	auto tok = fsv::filtered_string_view{"x"};
	auto v = fsv::split(sv, tok);
	auto expected = std::vector<fsv::filtered_string_view>{"", "", ""};

	CHECK(v == expected);
}
TEST_CASE("Split on single character with multiple with other characters") {
	auto sv = fsv::filtered_string_view{"xbx"};
	auto tok = fsv::filtered_string_view{"x"};
	auto v = fsv::split(sv, tok);
	auto expected = std::vector<fsv::filtered_string_view>{"", "b", ""};

	CHECK(v == expected);
}
TEST_CASE("Split with no delimiters present", "[split]") {
	auto sv = fsv::filtered_string_view{"abc"};
	auto tok = fsv::filtered_string_view{"x"};
	auto v = fsv::split(sv, tok);
	auto expected = std::vector<fsv::filtered_string_view>{"abc"};

	CHECK(v == expected);
}
TEST_CASE("Split with delimiter at the end", "[split]") {
	auto sv = fsv::filtered_string_view{"abx"};
	auto tok = fsv::filtered_string_view{"x"};
	auto v = fsv::split(sv, tok);
	auto expected = std::vector<fsv::filtered_string_view>{"ab", ""};

	CHECK(v == expected);
}
TEST_CASE("Split with delimiter at the start", "[split]") {
	auto sv = fsv::filtered_string_view{"xab"};
	auto tok = fsv::filtered_string_view{"x"};
	auto v = fsv::split(sv, tok);
	auto expected = std::vector<fsv::filtered_string_view>{"", "ab"};

	CHECK(v == expected);
}
TEST_CASE("Test substr on filtered_string_view", "[substr]") {
	auto sv = fsv::filtered_string_view{"Siberian Husky"};
	auto sub_sv = fsv::substr(sv, 9); // 从索引 9 开始获取子字符串
	auto result = static_cast<std::string>(sub_sv); // 将 filtered_string_view 转换为 std::string 来检查

	CHECK(result == "Husky"); // 检查是否正确获得 "Husky"
}
TEST_CASE("Test substr on filtered_string_view with custom predicate", "[substr]") {
	// 定义一个简单的谓词函数对象，只接受 'S' 和 'D'
	auto only_SD = [](const char& c) { return c == 'S' || c == 'D'; };
	// 创建 filtered_string_view 实例，过滤后的字符串应该是 "SD"
	fsv::filtered_string_view sv{"Sled Dog", only_SD};
	// 测试 substr 获取过滤后的字符串的子字符串
	auto sub_view = fsv::substr(sv, 0, 2);
	// 使用 CHECK 验证 substr 的结果是否符合预期
	CHECK(static_cast<std::string>(sub_view) == "SD");
}
TEST_CASE("Test substr on filtered_string_view with negative start", "[substr]") {
	auto sv = fsv::filtered_string_view{"Example"};
	auto sub_sv = fsv::substr(sv, -5); // Trying negative start index
	auto result = static_cast<std::string>(sub_sv);

	CHECK(result == "Example"); // As negative start should clamp to zero and return the whole string
}
TEST_CASE("Test substr with count exceeding filtered_string_view size", "[substr]") {
	auto sv = fsv::filtered_string_view{"Example"};
	auto sub_sv = fsv::substr(sv, 3, 20); // Start at index 3 with count exceeding size of string
	auto result = static_cast<std::string>(sub_sv);

	CHECK(result == "mple"); // Should return the substring from index 3 to end
}
TEST_CASE("Test substr on empty filtered_string_view", "[substr]") {
	auto sv = fsv::filtered_string_view{""};
	auto sub_sv = fsv::substr(sv, 0, 1); // Substring on an empty string
	auto result = static_cast<std::string>(sub_sv);

	CHECK(result.empty()); // Should return an empty string
}
TEST_CASE("Test substr on filtered_string_view with end boundary", "[substr]") {
	auto sv = fsv::filtered_string_view{"Example"};
	auto sub_sv = fsv::substr(sv, 4, 3); // Proper substring that fits exactly to the end
	auto result = static_cast<std::string>(sub_sv);

	CHECK(result == "ple"); // Should correctly handle the boundary condition
}
