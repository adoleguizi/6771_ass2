#include "./filtered_string_view.h"

// Implement here
// Example usage and assertion to check that all chars are accepted
int main() {
	for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); c++) {
		assert(fsv::filtered_string_view::default_predicate(c));
	}
	// Also include the max() character
	assert(fsv::filtered_string_view::default_predicate(std::numeric_limits<char>::max()));
	// needed to overlead the operator<< for filtered_string_view
	auto s = std::string{"cat"};
	auto sv = fsv::filtered_string_view{s};
	std::cout << sv.size() << std::endl;
}