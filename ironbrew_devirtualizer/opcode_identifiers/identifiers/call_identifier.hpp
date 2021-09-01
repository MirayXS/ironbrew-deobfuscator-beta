#pragma once

#include "../identifier_handler.hpp"

namespace deobf::ironbrew_devirtualizer::opcode_identifiers {
	using namespace ast;

	struct call_identifier final : public basic_opcode_handler {
		vm_arch::opcode handle(ir::statement::block* path) override {
			auto binary_exp = path->find_first_of<ir::expression::binary_expression>();
			if (binary_exp.has_value() && binary_exp->get().right->to_string() == "2") {
				return __super::handle(path);
			}

			if (!path->find_first_of<ir::statement::do_block>().has_value()) {
				auto function_call = path->find_first_of<ir::expression::function_call>();
				if (function_call.has_value()) {
					if (auto function_name = function_call->get().name; function_name.has_value()) {
						const auto string_name = function_name.value()->as<ir::expression::variable>();
						
						if (string_name->to_string() == "pack_return") {
							return vm_arch::opcode::op_call;
						}
						else if (string_name->name->to_string() == "stack" && string_name->suffixes.size() == 1 && string_name->suffixes.front()->name->to_string() == "instruction_opcode_a") { // A (call succesor)
							return vm_arch::opcode::op_call;
						}
					}
				}
			}

			return __super::handle(path);
		}
	};
}