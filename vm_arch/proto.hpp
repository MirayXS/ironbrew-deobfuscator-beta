#pragma once
#include "instruction.hpp"
#include "constant.hpp"
#include "opcode.hpp"

#include <vector>
#include <memory>
#include <iostream>

namespace deobf::vm_arch {
	struct proto {
		std::vector<std::unique_ptr<instruction>> instructions;
		std::vector<std::unique_ptr<proto>> protos;
		std::vector<double> lineinfo;
		std::vector<std::unique_ptr<constant>> constants;

		std::uint8_t num_params;
		std::uint16_t nups;
		double max_stack_size;
		bool is_vararg;

		std::string to_string() {
			auto sz = 0ul;
			for (auto& instr : instructions) {
				std::cout << '[' << sz << "] : " << vm_arch::opcode_map[instr->op] << ' ' << static_cast<long>(instr->a) << ' ' << static_cast<long>(instr->sbx) << ' ' << static_cast<long>(instr->c) << std::endl;
				sz++;
			}

			return "";
		}

		virtual ~proto() = default;
	};
}