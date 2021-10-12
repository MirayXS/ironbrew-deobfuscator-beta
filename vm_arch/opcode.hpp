#pragma once

#include <cstdint>
#include <unordered_map>
#include <string_view>

namespace deobf::vm_arch {
	enum class opcode : std::uint8_t { // https://www.lua.org/source/5.1/lopcodes.h.html
		op_invalid,

		op_move,
		op_loadk,
		op_loadbool,
		op_loadnil,
		op_getupval,
		op_getglobal,
		op_gettable,
		op_setglobal,
		op_setupval,
		op_settable,
		op_newtable,
		op_self,
		op_add,
		op_sub,
		op_mul,
		op_div,
		op_mod,
		op_pow,
		op_unm,
		op_not,
		op_len,
		op_concat,
		op_jmp,
		op_eq,
		op_lt,
		op_le,
		op_test,
		op_testset,
		op_call,
		op_tailcall,
		op_return,
		op_forloop,
		op_forprep,
		op_tforloop,
		op_setlist,
		op_close,
		op_closure,
		op_vararg,

		// custom vm opcodes
		op_newstack,
		op_settop,
	};

	static inline std::unordered_map<vm_arch::opcode, std::string_view> opcode_map {
		{ opcode::op_invalid, "invalid" },

		{ opcode::op_move, "move" },
		{ opcode::op_loadk, "loadk" },
		{ opcode::op_loadbool, "loadbool" },
		{ opcode::op_loadnil, "loadnil" },
		{ opcode::op_getupval, "getupval" },
		{ opcode::op_getglobal, "getglobal" },
		{ opcode::op_gettable, "gettable" },
		{ opcode::op_setglobal, "setglobal" },
		{ opcode::op_setupval, "setupval" },
		{ opcode::op_settable, "settable" },
		{ opcode::op_newtable, "newtable" },
		{ opcode::op_self, "self" },
		{ opcode::op_add, "add" },
		{ opcode::op_sub, "sub" },
		{ opcode::op_mul, "mul" },
		{ opcode::op_div, "div" },
		{ opcode::op_mod, "mod" },
		{ opcode::op_pow, "pow" },
		{ opcode::op_unm, "unm" },
		{ opcode::op_not, "not" },
		{ opcode::op_len, "len" },
		{ opcode::op_concat, "concat" },
		{ opcode::op_jmp, "jmp" },
		{ opcode::op_eq, "eq" },
		{ opcode::op_lt, "lt" },
		{ opcode::op_le, "le" },
		{ opcode::op_test, "test" },
		{ opcode::op_testset, "testset" },
		{ opcode::op_call, "call" },
		{ opcode::op_tailcall, "tailcall" },
		{ opcode::op_return, "return" },
		{ opcode::op_forloop, "forloop" },
		{ opcode::op_forprep, "forprep" },
		{ opcode::op_tforloop, "tforloop" },
		{ opcode::op_setlist, "setlist" },
		{ opcode::op_close, "close" },
		{ opcode::op_closure, "closure" },
		{ opcode::op_vararg, "vararg" },

		// custom vm opcodes
		{ opcode::op_newstack, "newstack" },
		{ opcode::op_settop, "settop" },
	};

	enum class instruction_mode {
		a, // 8 bits
		b, // 9 bits
		c, // 9 bits
		bx, // 18 bits (b+c)
		sbx, // signed bx
	};

	enum class instruction_type {
		abc,
		abx,
		asbx,
		asbxc,

		a,
		ab,
	};
}