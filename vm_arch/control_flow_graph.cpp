#include "control_flow_graph.hpp"

#include <stdexcept>
#include <iostream>

namespace deobf::vm_arch::control_flow_graph {


	std::shared_ptr<basic_block> generate_basic_blocks(std::unordered_map<std::size_t, std::shared_ptr<basic_block>>& block_references, std::size_t init_label, std::vector<std::unique_ptr<vm_arch::instruction>>& instructions, std::unordered_set<vm_arch::instruction*>& labels) {
		if (const auto result = block_references.find(init_label); result != block_references.cend()) {
			return result->second;
		}
		
		auto entry_block = std::make_shared<basic_block>();

		block_references.emplace(init_label, entry_block);

		for (; init_label < instructions.size(); ++init_label) {
			const auto& current_instruction = instructions.at(init_label);

			entry_block->instructions.emplace_back(*current_instruction);

			if (current_instruction->op == vm_arch::opcode::op_return) {
				std::cout << "retblock.\n";
				entry_block->is_terminator = true; // flip terminator flag (current terminator instructions : return)
				return entry_block;
			}
			else if (current_instruction->op == vm_arch::opcode::op_loadbool && current_instruction->c != 0) { // if c then pc++ (example : local a = (B ~= 2))
				// do nothing for now
			}
			else if (current_instruction->is_unconditional_jump()) {
				std::cout << "op:" << vm_arch::opcode_map[current_instruction->op] << std::endl;
				const auto target_pc = current_instruction->get_target_pc(init_label);

				if (target_pc < 0 || target_pc > instructions.size()) {
					throw std::runtime_error("un/conditional jump target pc is out of bounds");
				}
				std::cout << "target:" << target_pc << std::endl;
				const auto target_instruction = instructions.at(target_pc).get();
				if (labels.count(target_instruction)) {
					entry_block->target_block = generate_basic_blocks(block_references, target_pc, instructions, labels);
				}

				entry_block->next_block = generate_basic_blocks(block_references, init_label + 1, instructions, labels);
				// next basic block starts after current leader

				return entry_block;
			}
			else if (labels.count(current_instruction.get())) {
				std::cout << "populatenext\n";
				entry_block->next_block = generate_basic_blocks(block_references, init_label + 1, instructions, labels);
			}
		}

		return entry_block;
	}

	std::shared_ptr<basic_block> generate_graph(std::vector<std::unique_ptr<vm_arch::instruction>>& instructions) {
		std::unordered_set<vm_arch::instruction*> labels;
		std::cout << "generating graph\n";
		// identify labels
		labels.emplace(instructions.at(0).get()); // first instruction is always

		auto pc_loc = std::size_t{ };
		for (auto& instruction : instructions) {
			++pc_loc; // increment pc loc on start so we can terminate our basic block with a branch instruction.

			if (instruction->is_unconditional_jump()) {
				// get target of the goto instruction
				const auto target_pc = instruction->get_target_pc(pc_loc); // get the target address.
				if (target_pc > 0 && target_pc < instructions.size()) { // check if target in domain
					labels.emplace(instructions.at(target_pc).get());
				}

				// aswell as current instruction
				if (pc_loc < instructions.size()) {
					labels.emplace(instructions.at(pc_loc).get());
				}

			}
			else if (instruction->op == vm_arch::opcode::op_return) {
				if (pc_loc < instructions.size()) {
					labels.emplace(instructions.at(pc_loc).get()); // might have multiple returns
				}
			}
		}

		std::unordered_map<std::size_t, std::shared_ptr<basic_block>> reference_block_map;

		std::shared_ptr<basic_block> root_block = generate_basic_blocks(reference_block_map, 0, instructions, labels);

		std::cout << "terminator test : " << root_block->target_block->target_block->is_terminator << std::endl;
		for (auto& instr : root_block->next_block->instructions) {
			std::cout << "instr:" << vm_arch::opcode_map[instr.get().op] << ' ' << instr.get().a << ' ' << instr.get().b << ' '<< instr.get().c << std::endl;
		}

		return root_block;
	}
}