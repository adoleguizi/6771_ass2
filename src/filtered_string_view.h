#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <cassert>
#include <compare>
#include <functional>
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
		filtered_string_view()
		: data_(nullptr)
		, length_(0)
		, predicate_(default_predicate){};
		// size() implemantation
		auto size() const -> std::size_t {
			return length_;
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
