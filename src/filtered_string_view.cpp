#include "./filtered_string_view.h"

// Implement here
fsv::filtered_string_view::filtered_string_view()
: data_(nullptr)
, length_(0)
, predicate_(default_predicate){};

fsv::filtered_string_view::filtered_string_view(const std::string& str)
: data_(str.data())
, length_(str.size())
, predicate_(default_predicate){};

fsv::filtered_string_view::filtered_string_view(const std::string& str, filter predicate)
: data_(str.data())
, length_(str.size())
, predicate_(predicate){};
// Example usage and assertion to check that all chars are accepted
int main() {
	for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); c++) {
		assert(fsv::filtered_string_view::default_predicate(c));
	}
	// Also include the max() character
	assert(fsv::filtered_string_view::default_predicate(std::numeric_limits<char>::max()));
	// needed to overlead the operator<< for filtered_string_view

	//	auto sv = fsv::filtered_string_view{};
	//	std::cout << sv.size() << std::endl;

	//	auto s = std::string{"cat"};
	//	auto sv = fsv::filtered_string_view{s};
	//	std::cout << sv.size() << std::endl;
	auto s = std::string{"cat"};
	auto pred = [](const char& c) { return c == 'a'; };
	auto sv = fsv::filtered_string_view{s, pred};
	std::cout << sv.size() << std::endl;
}