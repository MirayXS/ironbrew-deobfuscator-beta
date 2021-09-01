#include "marker_decorator.hpp"

namespace deobf::ironbrew_devirtualizer::devirtualizer_markers {
	bool marker_decorator::accept(ir::expression::variable* expression) {
		if (auto symbol = current_block->find_symbol(expression->name->to_string())) {
			if (rename_list.count(symbol->resolve_identifier)) { // lazy again
				static_cast<ir::expression::string_literal*>(expression->name.get())->value = symbol->resolve_identifier;
			}
		}

		return true;
	}

	bool marker_decorator::accept(ir::statement::block* statement) {

		const auto old_block = current_block;
		
		current_block = statement;

		for (auto& stat : statement->body)
			stat->accept(this);

		if (statement->ret.has_value())
			statement->ret.value()->accept(this);

		current_block = old_block;

		return false;
	}
}