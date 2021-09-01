#pragma once
#include <array>
//#include <forward_list>

#include "./ast/ir/statement.hpp"

namespace deobf::ironbrew_devirtualizer::symbolic_execution::deserializer {
	using namespace ast;

	enum class process_order {
		instructions,
		protos,
		lineinfo,
		constants,
		parameters,
	};

	constexpr std::size_t max_process_order = 5;

	enum class constant_type : std::uint8_t {
		string,
		number,
		boolean,
	};

	constexpr std::size_t max_constant_order = 3;

	struct deserializer_context final {
		ir::statement::block* const root;

		std::vector<process_order> chunk_order{ }; // todo reserve for enum count?

		std::map<std::uint8_t, constant_type> constant_order_mapping{ };

		/*struct {
			constant_type string_order, boolean_order, number_order;
		};*/

		explicit deserializer_context(ir::statement::block* const root) :
			root(root)
		{
			chunk_order.reserve(max_process_order);
		}

		virtual ~deserializer_context() = default;
	};
}