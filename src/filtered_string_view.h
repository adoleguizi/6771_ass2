#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <algorithm>
#include <cassert>
#include <cctype>
#include <compare>
#include <cstddef>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
namespace fsv {
	using filter = std::function<bool(const char&)>;
	class filtered_string_view {
		class iter {
		 public:
			using MEMBER_TYPEDEFS_GO_HERE = void;
			// define the iterator

			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = char;
			using reference = const char&;
			using pointer = void;
			using difference_type = std::ptrdiff_t;

			//
			iter() = default;
			// Constructors
			iter(const char* ptr = nullptr, const filtered_string_view* container = nullptr) noexcept;

			auto operator*() const -> reference; // change this
			auto operator->() const -> pointer; // change this

			auto operator++() -> iter&;
			auto operator++(int) -> iter;
			auto operator--() -> iter&;
			auto operator--(int) -> iter;

			friend auto operator==(const iter& lhs, const iter& rhs) -> bool {
				// 先检查迭代器是否来自同一个容器
				return lhs.container_ == rhs.container_ && lhs.ptr_ == rhs.ptr_;
			}
			friend auto operator!=(const iter& rhs, const iter& lhs) -> bool {
				return !(lhs == rhs);
			}

		 private:
			/* Implementation-specific private members */
			const char* ptr_;
			const filtered_string_view* container_;

			friend class filtered_string_view;

		}; // iter

	 public:
		using iterator = iter;
		using const_iterator = iter;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		iterator begin() const noexcept;
		iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		reverse_iterator rbegin() const noexcept;
		reverse_iterator rend() const noexcept;
		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;
		static filter default_predicate;
		// default constructor initialize the data_ to nullptr, length_ to 0 and predicate_ to default_predicate
		filtered_string_view() noexcept;
		filtered_string_view(const std::string& str) noexcept;
		filtered_string_view(const std::string& str, filter predicate) noexcept;
		filtered_string_view(const char* str) noexcept;
		// NULL terminated string constructor with predicate constructor
		filtered_string_view(const char* str, filter predicate) noexcept;
		// add new constructor
		filtered_string_view(const char* data, size_t length, filter pred) noexcept;
		filtered_string_view(const filtered_string_view& other);
		filtered_string_view(filtered_string_view&& other) noexcept;
		~filtered_string_view();

		auto operator=(const filtered_string_view& other) -> filtered_string_view&;
		// Move assignment
		auto operator=(filtered_string_view&& other) noexcept -> filtered_string_view&;
		// Subscript
		auto operator[](int n) const noexcept -> const char&;
		// String Type Conversion
		explicit operator std::string() const;
		// at() implementation
		auto at(int index) const -> const char&;

		// size() implemantation
		auto size() const -> std::size_t;
		// empty() declaration
		auto empty() const -> bool;

		auto data() const noexcept -> const char*;

		auto predicate() const noexcept -> const filter&;

		std::vector<std::size_t> filtered_indices() const {
			std::vector<std::size_t> indices;
			for (std::size_t i = 0; i < length_; ++i) {
				if (predicate_(data_[i])) {
					indices.push_back(i);
				}
			}
			return indices;
		}

	 private:
		const char* data_;
		// set the length of view
		std::size_t length_;
		// set the predicate function (lambda function)
		filter predicate_;
		// friend filtered_string_view;
	}; // filter_string_view
	// Non-member operator
	// Equality operator
	auto operator==(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> bool;
	// Inequality operator
	auto operator!=(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> bool;
	// Relational operators
	auto operator<=>(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> std::strong_ordering;
	// ostream operator
	auto operator<<(std::ostream& os, const filtered_string_view& fsv) -> std::ostream&;
	// Non-member utility functions
	// compose
	auto compose(const filtered_string_view& fsv, const std::vector<filter>& filts) noexcept -> filtered_string_view;
	// split utility function
	auto split(const filtered_string_view& fsv, const filtered_string_view& tok) -> std::vector<filtered_string_view>;
	// substring utility function
	auto substr(const filtered_string_view& fsv, int pos = 0, int count = 0) noexcept -> filtered_string_view;

} // namespace fsv

#endif // COMP6771_ASS2_FSV_H
