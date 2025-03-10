#pragma once

#include "../identifier_handler.hpp"

/*
artimethic OP forms in ironbrew:
	stack[instruction_opcode_a] = ( instruction_opcode_b OP instruction_opcode_c )

	stack[instruction_opcode_a] = ( stack[instruction_opcode_b] OP instruction_opcode_c )

	stack[instruction_opcode_a] = ( instruction_opcode_b OP stack[instruction_opcode_c] )

	stack[instruction_opcode_a] = ( stack[instruction_opcode_b] OP stack[instruction_opcode_c] )
*/

namespace deobf::ironbrew_devirtualizer::opcode_identifiers {
	using namespace ast;

	struct div_identifier final : public basic_opcode_handler {
		vm_arch::opcode handle(ir::statement::block* path) override {
			if (path->body.size() == 1 && path->body.at(0)->is<ir::statement::variable_assign>()) {
				const auto assign_string = path->body.at(0)->to_string();
				if (assign_string == "stack[instruction_opcode_a] = ( instruction_opcode_b / instruction_opcode_c )"
					|| assign_string == "stack[instruction_opcode_a] = ( stack[instruction_opcode_b] / instruction_opcode_c )"
					|| assign_string == "stack[instruction_opcode_a] = ( instruction_opcode_b / stack[instruction_opcode_c] )"
					|| assign_string == "stack[instruction_opcode_a] = ( stack[instruction_opcode_b] / stack[instruction_opcode_c] )") {
					return vm_arch::opcode::op_div;
				}
			}

			return __super::handle(path);
		}
	};
}