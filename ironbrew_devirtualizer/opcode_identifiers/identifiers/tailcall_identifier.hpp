#pragma once

#include "../identifier_handler.hpp"

namespace deobf::ironbrew_devirtualizer::opcode_identifiers {
	using namespace ast;

	struct tailcall_identifier final : public basic_opcode_handler {
		vm_arch::opcode handle(ir::statement::block* path) override {
			auto do_statement = path->find_first_of<ir::statement::do_block>();
			if (do_statement.has_value()) {
				auto& body_wrapper = do_statement->get();
				if (body_wrapper.body->body.size() == 0) {
					auto call_statement = body_wrapper.find_first_of<ir::expression::function_call>();
					if (call_statement.has_value()) {
						auto& call_wrapper = call_statement->get();
						if (auto variable_name = call_wrapper.name.value()->as<ir::expression::variable>()) {
							if (variable_name->to_string() == "stack[instruction_opcode_a]") {
								//std::cout << "tailcal detected\n";
								return vm_arch::opcode::op_tailcall;
							}
						}
					}
				}
			}

			return __super::handle(path);
		}
	};
}