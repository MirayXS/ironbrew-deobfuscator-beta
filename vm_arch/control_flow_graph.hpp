#pragma once

#include "basic_block.hpp"
#include "instruction.hpp"

#include <memory>
#include <unordered_set>
#include <set>

namespace deobf::vm_arch::control_flow_graph {

	using instruction_type = std::reference_wrapper<vm_arch::instruction>;

	// generates a DAG based on basic blocks.
	// returns a entry block

	std::shared_ptr<basic_block> generate_basic_blocks(std::unordered_map<std::size_t, std::shared_ptr<basic_block>>& block_references, std::size_t init_label, std::vector<std::unique_ptr<vm_arch::instruction>> & instructions, std::unordered_set<vm_arch::instruction*>& labels);
	
	
	std::shared_ptr<basic_block> generate_graph(std::vector<std::unique_ptr<vm_arch::instruction>>& instructions);
}