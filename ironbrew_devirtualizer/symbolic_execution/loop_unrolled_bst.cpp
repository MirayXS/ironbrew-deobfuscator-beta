#pragma once

#include "./ast/ir/abstract_visitor_pattern.hpp"
#include "./ast/ir/node.hpp"
#include "loop_unrolled_bst.hpp"
#include "./ironbrew_devirtualizer/opcode_identifiers/identifier_base.hpp"

#include <algorithm>
#include <numeric>

namespace deobf::ironbrew_devirtualizer::symbolic_execution {
	using namespace ast;

	//decltype(loop_unrolled_bst::callback_functor) loop_unrolled_bst::callback_handler;

	bool loop_unrolled_bst::accept(ir::expression::binary_expression* expression) {
		if (back_track.empty())
			return false;

		if (auto left_value = expression->left->as<ir::expression::variable>()) {
			if (left_value->to_string() != "virtual_opcode") {
				return false;
			}
		}

		if (auto right_value = expression->right->as<ir::expression::numeral_literal>()) {
			using operation_t = typename ir::expression::binary_expression::operation_t;

			const auto virtual_opcode = back_track.front().get().virtual_opcode;

			switch (expression->operation) { // concerete execute for bound
				case operation_t::le:
					return virtual_opcode <= right_value->value; // this is a divide and conquer route, continue to next if statements
				case operation_t::gt:
					return virtual_opcode > right_value->value;
				case operation_t::eq:
					return virtual_opcode == right_value->value;
			}
		}

		return false;
	}

	bool loop_unrolled_bst::accept(ir::statement::block* body)  {
		if (body->body.size() == 1) {
			if (auto if_statement = body->body.at(0)->as<ir::statement::if_statement>()) { // check node of tree in BST
				if (auto if_branch = if_statement->condition->as<ir::expression::binary_expression>()) {
					if (auto left_string = if_branch->left->as<ir::expression::variable>()) {
						if (left_string->to_string() == "virtual_opcode") {
							return true;
						}
					}
				}
			}
		}

		if (back_track.empty()) {
			return false;
		}

		// we have entered the final block, map it

		if (auto memoized_result = memoized_virtuals.find(back_track.front().get().virtual_opcode); memoized_result != memoized_virtuals.cend()) {
			std::cout << "got top:" << static_cast<int>(memoized_result->first) << std::endl;
			for (auto& opcode : memoized_result->second) {
				if (back_track.empty())
					break;

				auto front_instruction = back_track.front();
				front_instruction.get().op = opcode;

				std::invoke(callback_functor, front_instruction, nullptr);
				back_track.pop_front();
			}
		}
		else {
			//std::cout << body->body.at(0)->to_string() << std::endl;
			// call super op handler
			std::cout << "front instr:" << back_track.front().get().virtual_opcode << std::endl;
			handle(back_track.front(), body);
		}
		//std::cout << "done." << std::endl;

		run_cycle();

		return false;
	}

	bool loop_unrolled_bst::accept(ir::statement::if_statement* if_stat) { // i couldve shorted this into 1 line, but changing the visitor pattern itself from the IR will affect other things.

		const auto root_condition = static_cast<ir::expression::binary_expression*>(if_stat->condition.get());

		if (accept(root_condition)) {
			if_stat->body->accept(this);
			return false;
		}

		for (const auto& [condition, body] : if_stat->else_if_statements) {
			const auto binary_condition = static_cast<ir::expression::binary_expression*>(condition.get());
			if (accept(binary_condition)) {
				body->accept(this);
				return false;
			}
		}

		if (if_stat->else_body.has_value()) {
			if (accept(if_stat->else_body.value().get())) {
				if_stat->body->accept(this);
				return false;
			}
		}

		return true;
	}

}
