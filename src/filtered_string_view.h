#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <compare>
#include <functional>
#include <iterator>
#include <optional>
#include <string>

namespace fsv {
	using filter = std::function<bool(const char&)>;
	class filtered_string_view {
		static filter default_predicate;
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
	 private:
	}; // filter_string_view
	// static member definition outside class
	filter filtered_string_view::default_predicate = [](const char& c) -> bool {
		(void)c;
		return true; // default predicate that accepts all chracters
	};
} // namespace fsv

#endif // COMP6771_ASS2_FSV_H
