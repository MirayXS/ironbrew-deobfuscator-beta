#pragma once

#include "./ast/optimization_passes/base_optimizer.hpp"
#include "./ast/ir/abstract_visitor_pattern.hpp"
#include "./ast/ir/node.hpp"
#include "./ast/ir/statement.hpp"

#include <set>
#include <string_view>

//#include "ironbrew_devirtualizer/devirtualizer_context/devirtualizer_context.hpp"

namespace deobf::ironbrew_devirtualizer::devirtualizer_markers {
	using namespace ast;

	struct marker_decorator : ir::abstract_visitor_pattern { // decorator design pattern on visitor.
		ir::statement::block* current_block = nullptr;

		bool accept(ir::expression::variable* expression) override;
		bool accept(ir::statement::block* statement) override;

		explicit marker_decorator(const std::initializer_list<std::string_view>& renames) {
			rename_list.insert(renames);
		}

	protected:
		std::set<std::string_view> rename_list;
	};
}