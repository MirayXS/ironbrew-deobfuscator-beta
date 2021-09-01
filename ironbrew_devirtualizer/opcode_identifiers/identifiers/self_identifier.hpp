#pragma once

#include "../identifier_handler.hpp"

namespace deobf::ironbrew_devirtualizer::opcode_identifiers {
	using namespace ast;

	struct self_identifier final : public basic_opcode_handler {
		vm_arch::opcode handle(ir::statement::block* path) override {
			if (auto variable_assign = path->find_first_of<ir::statement::variable_assign>(); variable_assign.has_value()) {
				if (variable_assign.value().get().variables.size() == 1) {
					auto first_variable = variable_assign.value().get().variables.at(0);
					if (auto variable = first_variable->as<ir::expression::variable>()) {
						if (variable->to_string() == "stack[( instruction_opcode_a + 1 )]") {
							return vm_arch::opcode::op_self;
						}
					}
				}
			}

			return __super::handle(path);
		}
	};
}