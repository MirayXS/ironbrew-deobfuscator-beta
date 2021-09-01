#pragma once
#include <cstdint>
#include "opcode.hpp"

namespace deobf::vm_arch {
#pragma pack(push, 1)
	struct instruction final {
		vm_arch::opcode op{ };
		double virtual_opcode;
		std::uint16_t a;
		std::uint16_t b;
		std::uint16_t c;
		std::uint32_t bx;
		std::int32_t sbx;

		/*union { // truncated to 2 bytes
			struct {
				std::uint16_t b;
				std::uint16_t c;
			};
			std::uint32_t bx;
			std::int32_t sbx;
		};*/


		explicit instruction(std::uint16_t virtual_opcode, std::uint8_t a) :
			virtual_opcode(virtual_opcode),
			a(a)
		{ }

		// https://the-ravi-programming-language.readthedocs.io/en/latest/lua_bytecode_reference.html

		// tforloop and forloop and forprep and jmp : pcloc + sbx
		// eq, lt, le : pc++
		// test, testtest (conditional) : pc++

		std::size_t get_target_pc(std::size_t pc_loc) const {
			if (op == vm_arch::opcode::op_tforloop || op == vm_arch::opcode::op_forloop || op == vm_arch::opcode::op_forprep || op == vm_arch::opcode::op_jmp) {
				return sbx; //pc_loc += sbx;
			}
			else if (op == vm_arch::opcode::op_eq || op == vm_arch::opcode::op_lt || op == vm_arch::opcode::op_le) {
				return ++pc_loc;
			}
			else if (op == vm_arch::opcode::op_test || op == vm_arch::opcode::op_testset) {
				return ++pc_loc;
			}
		}

		bool is_unconditional_jump() const {
			return (op == vm_arch::opcode::op_jmp ||
				op == vm_arch::opcode::op_eq ||
				op == vm_arch::opcode::op_lt ||
				op == vm_arch::opcode::op_le ||
				op == vm_arch::opcode::op_test ||
				op == vm_arch::opcode::op_testset ||
				op == vm_arch::opcode::op_forloop ||
				op == vm_arch::opcode::op_tforloop ||
				op == vm_arch::opcode::op_forprep);
		}

		instruction(const instruction&) = delete;
		instruction& operator=(const instruction&) = delete;

		virtual ~instruction() = default;
	};
#pragma pack(pop)
}