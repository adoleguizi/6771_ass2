#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <cassert>
#include <compare>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <string>
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
		const char* data_;
		// set the length of view
		std::size_t length_;
		// set the predicate function (lambda function)
		filter predicate_;
		static filter default_predicate;
		// default constructor initialize the data_ to nullptr, length_ to 0 and predicate_ to default_predicate
		filtered_string_view();
		filtered_string_view(const std::string& str);
		filtered_string_view(const std::string& str, filter predicate);

		// size() implemantation
		auto size() const -> std::size_t {
			std::size_t count = 0;
			for (std::size_t i = 0; i < length_; ++i) {
				if (predicate_(data_[i])) {
					++count;
				}
			}
			return count;
		}

		auto data() const -> const char* {
			return data_;
		}

	 private:
	}; // filter_string_view
	// static member definition outside class
	filter filtered_string_view::default_predicate = [](const char& c) -> bool {
		(void)c;
		return true; // default predicate that accepts all chracters
	};
} // namespace fsv

#endif // COMP6771_ASS2_FSV_H
