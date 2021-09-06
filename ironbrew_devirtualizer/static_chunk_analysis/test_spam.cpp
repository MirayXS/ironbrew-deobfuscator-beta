#include "test_spam.hpp"
#include <iostream>

namespace deobf::ironbrew_devirtualizer::static_chunk_analysis {
	using namespace vm_arch;

	namespace test_spam {
		void undo_test_spam(std::shared_ptr<basic_block>& block_instructions) {
			if (block_instructions->instructions.size() != 1) {
				return;
			}

			// iterate all target blocks?

			auto first_instruction = block_instructions->instructions.at(0);
			switch (first_instruction.get().op) {
				case opcode::op_eq: {
					std::cout << "eqlol\n";
					break;
				}
			}
		}
		
		void run(control_flow_graph::block_reference_t& reference_map) {
			for (auto& block : reference_map) {
				undo_test_spam(block.second);
			}
		}
	}
}

