#include "dynamic_chunk_analysis.hpp"

#include <algorithm>

namespace deobf::ironbrew_devirtualizer::dynamic_chunk_analysis {
	extern void run_analysis_session(vm_arch::proto* const current_proto) {

		// populate num upvalues (todo is this slower than the other approach?)
		const auto max_upvalue_instruction = std::max_element(current_proto->instructions.cbegin(), current_proto->instructions.cend(), [](const auto& maximum, const auto& current) {
			// B + 1 = upvalue size
			if (current->op == vm_arch::opcode::op_setupval || current->op == vm_arch::opcode::op_getupval) {
				return maximum->b < current->b;
			}

			return false;
		});

		if (max_upvalue_instruction->get()->op == vm_arch::opcode::op_setupval || max_upvalue_instruction->get()->op == vm_arch::opcode::op_getupval) {
			current_proto->nups = max_upvalue_instruction->get()->b + 1;
			std::cout << "maxupvalue:" << current_proto->nups << std::endl;
		}

		// populate vararg flag
		for (auto& instruction : current_proto->instructions) {
			if (instruction->op == vm_arch::opcode::op_vararg) {
				current_proto->is_vararg = true;
				break;
			}
		}

		// populate stack size (todo fix)
		std::size_t stack_size{ };

		for (auto& instruction : current_proto->instructions) {
			if (instruction->a > stack_size) {
				stack_size = instruction->a;
			}
		}

		current_proto->max_stack_size = stack_size; // parameters reserve registers in the stack aswell
		
		if (current_proto->num_params == 0) { // 1 free slot?
			current_proto->max_stack_size++;
		}

		std::cout << "stacksz:" << current_proto->max_stack_size << std::endl;

		for (auto& proto : current_proto->protos) {
			run_analysis_session(proto.get());
		}
	}
}