#pragma once
#include <variant>
#include <string>

#include <type_traits>

// credits to stack overflow for the is_contained pattern

template <typename T, typename ...>
struct is_contained : std::false_type { };

template <typename T, typename Head, typename ...Tail>
struct is_contained<T, Head, Tail...> : std::integral_constant<bool, std::is_same<T, Head>::value || is_contained<T, Tail...>::value> { };

namespace deobf::vm_arch {
	struct constant {
		enum class constant_type : short {
			nil, // todo do we really have to hold NIL ?

			number,
			string,
			boolean,
		};

		std::variant<std::nullptr_t, double, std::string, bool> value;

		template <typename kst_type,
			typename = std::enable_if<is_contained<typename std::decay<kst_type>::type, std::nullptr_t, double, std::string, bool>::value>>

		explicit constant(kst_type&& value) :
			value(value)
		{ }


		[[nodiscard]] inline auto get_constant_type() const noexcept {
			return static_cast<enum constant_type>(value.index());
		}
		
		virtual ~constant() = default;
	};
}