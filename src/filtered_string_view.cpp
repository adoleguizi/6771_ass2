#include <utility>

#include "./filtered_string_view.h"

// Implement here
fsv::filtered_string_view::filtered_string_view()
: data_(nullptr)
, length_(0)
, predicate_(default_predicate){};
fsv::filter fsv::filtered_string_view::default_predicate = [](const char& c) -> bool {
	(void)c;
	return true; // default predicate that accepts all chracters
};

fsv::filtered_string_view::filtered_string_view(const std::string& str)
: data_(str.data())
, length_(str.size())
, predicate_(default_predicate){};

fsv::filtered_string_view::filtered_string_view(const std::string& str, filter predicate)
: data_(str.data())
, length_(str.size())
, predicate_(std::move(predicate)){};
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