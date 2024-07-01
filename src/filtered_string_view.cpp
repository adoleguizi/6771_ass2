
#include "./filtered_string_view.h"
#include <string_view>
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
// Null-Terminated String with Predicate Constructor
fsv::filtered_string_view::filtered_string_view(const char* str, filter predicate) noexcept
: data_(str)
, length_(std::char_traits<char>::length(str))
, predicate_(std::move(predicate)){};

//  Copy and Move Constructors
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
// move assignment
auto fsv::filtered_string_view::operator=(filtered_string_view&& other) noexcept -> filtered_string_view& {
	// check for self assignment
	if (this != &other) {
		// using std::exchange move other and other resouse to default
		data_ = std::exchange(other.data_, nullptr);
		length_ = std::exchange(other.length_, 0);
		predicate_ = std::exchange(other.predicate_, default_predicate);
	}
	return *this;
}
// Subscript not requires bounds checking add noexcept read only const function
auto fsv::filtered_string_view::operator[](int n) const noexcept -> const char& {
	int count = 0;
	for (std::size_t i = 0; i < length_; ++i) {
		if (predicate_(data_[i])) {
			if (count == n) {
				return data_[i];
			}
			++count;
		}
	}
	return data_[0];
}
// String Type Conversion
fsv::filtered_string_view::operator std::string() const {
	std::string result;
	result.reserve(length_);
	for (std::size_t i = 0; i < length_; ++i) {
		if (predicate_(data_[i])) {
			result.push_back(data_[i]);
		}
	}
	return result;
}