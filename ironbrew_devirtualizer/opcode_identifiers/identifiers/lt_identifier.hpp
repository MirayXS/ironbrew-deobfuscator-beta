#pragma once

#include "../identifier_handler.hpp"

namespace deobf::ironbrew_devirtualizer::opcode_identifiers {
	using namespace ast;

	struct lt_identifier final : public basic_opcode_handler {
		vm_arch::opcode handle(ir::statement::block* path) override {
			if (path->body.size() == 1) {
				if (auto if_stat = path->body.at(0)->as<ir::statement::if_statement>()) {
					const auto expression_string = if_stat->condition->to_string();
					if (expression_string == "( instruction_opcode_a < instruction_opcode_c )"
						|| expression_string == "( stack[instruction_opcode_a] < instruction_opcode_c )"
						|| expression_string == "( instruction_opcode_a < stack[instruction_opcode_c] )"
						|| expression_string == "( stack[instruction_opcode_a] < stack[instruction_opcode_c] )") {
						return vm_arch::opcode::op_lt;
					}
				}
			}

			return __super::handle(path);
		}
	};
}