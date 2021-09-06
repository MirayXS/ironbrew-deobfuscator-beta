#pragma once

#include <variant>
#include <string>
#include <memory>
#include <functional>
#include <map>

#include "instruction.hpp"


namespace deobf::vm_arch {
	struct basic_block final {
		bool is_terminator; // ends with a return, terminator block

		std::vector<std::reference_wrapper<instruction>> instructions;

		std::map<std::size_t, std::shared_ptr<basic_block>> references;

		std::shared_ptr<basic_block> next_block;
		std::shared_ptr<basic_block> target_block;

		virtual ~basic_block() = default;
	};
}