
#include "./filtered_string_view.h"

// Implement here
fsv::filtered_string_view::filtered_string_view() noexcept
: data_(nullptr)
, length_(0)
, predicate_(default_predicate){};
fsv::filter fsv::filtered_string_view::default_predicate = [](const char& c) -> bool {
	(void)c;
	return true; // default predicate that accepts all chracters
};
// Implicit String Constructor
fsv::filtered_string_view::filtered_string_view(const std::string& str) noexcept
: data_(str.data())
, length_(str.size())
, predicate_(default_predicate){};
// String Constructor with Predicate
fsv::filtered_string_view::filtered_string_view(const std::string& str, filter predicate) noexcept
: data_(str.data())
, length_(str.size())
, predicate_(std::move(predicate)){};
// Implicit Null-Terminated String Constructor
fsv::filtered_string_view::filtered_string_view(const char* str) noexcept
: data_(str)
, length_(std::char_traits<char>::length(str))
, predicate_(default_predicate){};

// // Example usage and assertion to check that all chars are accepted
fsv::filtered_string_view::filtered_string_view(const filtered_string_view& other)
: data_(other.data_)
, length_(other.length_)
, predicate_(other.predicate_){};
// move constructor
fsv::filtered_string_view::filtered_string_view(filtered_string_view&& other) noexcept
: data_(std::exchange(other.data_, nullptr))
, length_(std::exchange(other.length_, 0))
, predicate_(std::exchange(other.predicate_, default_predicate)){};
// destructor
fsv::filtered_string_view::~filtered_string_view() = default;
// assignment operator
auto fsv::filtered_string_view::operator=(const filtered_string_view& other) -> filtered_string_view& {
	if (this != &other) {
		data_ = other.data_;
		length_ = other.length_;
		predicate_ = other.predicate_;
	}
	// if self assignment do nothing
	return *this;
}