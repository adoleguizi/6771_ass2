
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
, predicate_(predicate){};
// Implicit Null-Terminated String Constructor
fsv::filtered_string_view::filtered_string_view(const char* str) noexcept
: data_(str)
, length_(std::char_traits<char>::length(str))
, predicate_(default_predicate){};
// Null-Terminated String with Predicate Constructor
fsv::filtered_string_view::filtered_string_view(const char* str, filter predicate) noexcept
: data_(str)
, length_(std::char_traits<char>::length(str))
, predicate_(predicate){};

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
	std::string result = {};
	result.reserve(length_);
	for (std::size_t i = 0; i < length_; ++i) {
		if (predicate_(data_[i])) {
			result.push_back(data_[i]);
		}
	}
	return result;
}
// at() implementation with bounds checking and exception handling
auto fsv::filtered_string_view::at(int index) const -> const char& {
	if (index < 0 or static_cast<std::size_t>(index) >= length_ or length_ == 0) {
		throw std::domain_error("filtered_string_view::at(" + std::to_string(index) + "): invalid index");
	}
	int count = 0;
	for (std::size_t i = 0; i < length_; ++i) {
		if (predicate_(data_[i])) {
			if (count == index) {
				return data_[i];
			}
			++count;
		}
	}
	throw std::domain_error("filtered_string_view::at(" + std::to_string(index) + "): invalid index");
}
// size() implementation
auto fsv::filtered_string_view::size() const -> std::size_t {
	std::size_t count = 0;
	for (std::size_t i = 0; i < length_; ++i) {
		if (predicate_(data_[i])) {
			++count;
		}
	}
	return count;
}
// empty() implementation
auto fsv::filtered_string_view::empty() const -> bool {
	return size() == 0;
}
// data implmentation
auto fsv::filtered_string_view::data() const noexcept -> const char* {
	return data_;
}
auto fsv::filtered_string_view::predicate() const noexcept -> const filter& {
	return predicate_;
}
// Non-member operator
auto fsv::operator==(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> bool {
	std::string ls = lhs.operator std::string();
	std::string rs = rhs.operator std::string();
	return ls == rs;
}
// Inequality operator
auto fsv::operator!=(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> bool {
	return !(lhs == rhs);
}
// Relational Comparison
auto fsv::operator<=>(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> std::strong_ordering {
	std::string ls = lhs.operator std::string();
	std::string rs = rhs.operator std::string();
	return ls <=> rs;
}
// Output stream
auto fsv::operator<<(std::ostream& os, const filtered_string_view& fsv) -> std::ostream& {
	std::string filtered = static_cast<std::string>(fsv);
	os << filtered;
	return os;
}
// Non-member utility functions
// compose
auto fsv::compose(const filtered_string_view& fsv, const std::vector<filter>& filts) noexcept -> filtered_string_view {
	filter combined_filter = [filts](char c) {
		for (const auto& filt : filts) {
			if (!filt(c)) {
				return false; // Short-circuit: if one filter returns false, the combined filter returns false
			}
		}
		return true; // If all filters return true, the combined filter returns true
	};
	// Return a new filtered_string_view using the original string data and the combined filter
	return filtered_string_view(fsv.data(), combined_filter);
}

fsv::filtered_string_view::filtered_string_view(const char* data, size_t length, filter pred) noexcept
: data_(data)
, length_(length)
, predicate_(pred) {}

auto fsv::split(const filtered_string_view& fsv, const filtered_string_view& tok) -> std::vector<filtered_string_view> {
	std::vector<filtered_string_view> parts;
	if (tok.size() == 0) {
		parts.push_back(fsv);
		return parts;
	}

	const char* base = fsv.data();
	const char* current = base;
	const char* end = base + strlen(base); // using strlen to get the length of the string
	const char* delim_start = tok.data();
	size_t delim_size = strlen(delim_start); // using strlen to get the length of the string

	while (current < end) {
		const char* found = std::search(current, end, delim_start, delim_start + delim_size);
		if (found != current) {
			parts.emplace_back(current, found - current, fsv.predicate()); // add current part
		}
		else {
			parts.emplace_back("", 0, fsv.predicate()); // deal with empty part
		}
		current = found + delim_size;
		if (found == end)
			break;
	}
	if (current < end) {
		parts.emplace_back(current, end - current, fsv.predicate());
	}
	else if (current == end && delim_size > 0 && end != base) {
		// 处理字符串末尾是分隔符的情况
		parts.emplace_back("", 0, fsv.predicate());
	}
	return parts.empty() ? std::vector<filtered_string_view>{fsv} : parts;
}
// subscript utility fucntion
auto fsv::substr(const filtered_string_view& fsv, int pos, int count) noexcept -> filtered_string_view {
	if (pos < 0) {
		pos = 0;
	}
	if (static_cast<size_t>(pos) >= fsv.size()) {
		return filtered_string_view("", 0, fsv.predicate());
	}
	std::vector<size_t> indices = fsv.filtered_indices();
	if (indices.empty() || pos >= static_cast<int>(indices.size())) {
		return filtered_string_view("", 0, fsv.predicate());
	}
	size_t start_index = indices[static_cast<size_t>(pos)];
	size_t end_index = (count > 0 && static_cast<size_t>(pos + count) < indices.size())
	                       ? indices[static_cast<size_t>(pos + count) - 1] + 1
	                       : indices.back() + 1;
	return filtered_string_view(fsv.data() + start_index, end_index - start_index, fsv.predicate());
}
// iterator personal constructor
fsv::filtered_string_view::iter::iter(const char* ptr, const filtered_string_view* container) noexcept
: ptr_(ptr)
, container_(container) {}
// iterator default constructor
auto fsv::filtered_string_view::iter::operator*() const -> reference {
	return *ptr_;
}
//-> operator
auto fsv::filtered_string_view::iter::operator->() const -> pointer {
	return;
}
auto fsv::filtered_string_view::iter::operator++() -> iter& {
	// assume ptr is a pointer to current character
	if (ptr_ and ptr_ < container_->data_ + container_->length_) { // 确保不是空指针且未到达终结符
		do {
			++ptr_; // 移动到下一个字符
		} while (ptr_ < container_->data_ + container_->length_
		         and !container_->predicate_(*ptr_)); // 继续移动直到找到符合谓词的字符或到达字符串末尾
	}
	return *this;
}
auto fsv::filtered_string_view::iter::operator++(int) -> iter {
	auto temp = *this;
	++*this;
	return temp; // 返回自增前的迭代器
}
auto fsv::filtered_string_view::iter::operator--() -> iter& {
	// assume ptr is a pointer to current character
	if (ptr_ > container_->data_) { // 确保不是空指针且未到达终结符
		do {
			--ptr_; // 移动到下一个字符
		} while (ptr_ >= container_->data_
		         and !container_->predicate_(*ptr_)); // 继续移动直到找到符合谓词的字符或到达字符串末尾
	}
	return *this;
}
auto fsv::filtered_string_view::iter::operator--(int) -> iter {
	auto temp = *this;
	--*this;
	return temp; // 返回自增前的迭代器
}

// iterator start end
auto fsv::filtered_string_view::begin() const noexcept -> iter {
	const char* start = data_;
	while (start < data_ + length_ and !predicate_(*start)) {
		++start;
	}
	return iter(start, this);
}
auto fsv::filtered_string_view::end() const noexcept -> iter {
	return iter(data_ + length_, this);
}
auto fsv::filtered_string_view::cbegin() const noexcept -> const_iterator {
	return begin();
}

auto fsv::filtered_string_view::cend() const noexcept -> const_iterator {
	return end();
}
auto fsv::filtered_string_view::rbegin() const noexcept -> reverse_iterator {
	return reverse_iterator(end());
}

auto fsv::filtered_string_view::rend() const noexcept -> reverse_iterator {
	return reverse_iterator(begin());
}
auto fsv::filtered_string_view::crbegin() const noexcept -> const_reverse_iterator {
	return const_reverse_iterator(end());
}

auto fsv::filtered_string_view::crend() const noexcept -> const_reverse_iterator {
	return const_reverse_iterator(begin());
}
