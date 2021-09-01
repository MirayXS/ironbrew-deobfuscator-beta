#pragma once

#include "../identifier_handler.hpp"

namespace deobf::ironbrew_devirtualizer::opcode_identifiers {
	using namespace ast;

	struct setlist_identifier final : public basic_opcode_handler {
		vm_arch::opcode handle(ir::statement::block* path) override {
			auto function_call = path->find_first_of<ir::expression::function_call>();
			if (function_call.has_value()) {
				if (auto function_name = function_call->get().name; function_name.has_value()) {
					if (function_name.value()->to_string() == "table.insert") {
						return vm_arch::opcode::op_setlist;
					}
				}
			}

			return __super::handle(path);
		}
	};
}