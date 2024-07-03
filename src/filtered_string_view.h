#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <algorithm>
#include <cassert>
#include <cctype>
#include <compare>
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
namespace fsv {
	using filter = std::function<bool(const char&)>;
	class filtered_string_view {
		class iter {
		 public:
			using MEMBER_TYPEDEFS_GO_HERE = void;

			iter();

			auto operator*() const -> void; // change this
			auto operator->() const -> void; // change this

			auto operator++() -> iter&;
			auto operator++(int) -> iter;
			auto operator--() -> iter&;
			auto operator--(int) -> iter;

			friend auto operator==(const iter&, const iter&) -> bool;
			friend auto operator!=(const iter&, const iter&) -> bool;

		 private:
			/* Implementation-specific private members */
		}; // iter

	 public:
		// set start pointer to string or data

		static filter default_predicate;
		// default constructor initialize the data_ to nullptr, length_ to 0 and predicate_ to default_predicate
		filtered_string_view() noexcept;
		filtered_string_view(const std::string& str) noexcept;
		filtered_string_view(const std::string& str, filter predicate) noexcept;
		filtered_string_view(const char* str) noexcept;
		// NULL terminated string constructor with predicate constructor
		filtered_string_view(const char* str, filter predicate) noexcept;
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

	 private:
		const char* data_;
		// set the length of view
		std::size_t length_;
		// set the predicate function (lambda function)
		filter predicate_;
		// friend operator
		// auto operator==(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> bool;
		// auto operator!=(const filtered_string_view& lhs, const filtered_string_view& rhs) noexcept -> bool;
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

} // namespace fsv

#endif // COMP6771_ASS2_FSV_H
