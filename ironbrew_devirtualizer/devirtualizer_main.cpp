#include "devirtualizer_main.hpp"
#include <deque>
#include <functional>

#include "devirtualizer_markers/mark_deserializer.hpp"
#include "devirtualizer_markers/mark_binary_functions.hpp"
#include "symbolic_execution/deserializer/symbolic_deserializer.hpp"
#include "devirtualizer_markers/mark_wrapper.hpp"
#include "opcode_identifiers/identifier_base.hpp"
#include "symbolic_execution/deserializer/deserializer_emulator_main.hpp"
#include "dynamic_chunk_analysis/dynamic_chunk_analysis.hpp"

#include "../vm_arch/control_flow_graph.hpp"


// control flow optimizations
#include "static_chunk_analysis/test_spam.hpp"

namespace deobf::ironbrew_devirtualizer {
	using namespace ast;

	void ironbrew_devirtualizer::proccess_chunk(vm_arch::proto* proto) {
		for (const auto& opcode : proto->instructions) {
			search_tree->back_track.emplace_back(*opcode.get());
		}

		search_tree->run_cycle();

		for (auto& proto : proto->protos) {
			proccess_chunk(proto.get());
		}
	}

	void ironbrew_devirtualizer::devirtualize() {
		// some testings and warm up

		/*
			 OP index : 1
			 A index : 2
			 B index : 3
			 C index : 4
			 we dont need to automate this since in aztupbrew its static
		*/

		// locate context
		/*const auto statement = context->ret.value();

		const auto function_call = statement->find_first_of<ir::expression::function_call>().value().get();*/

		const auto entry_point = context; //function_call.name.value()->as<ir::expression::function>()->body;

		// initialize a devirtualizer context object

		//auto devirtualizer_context = std::make_shared<deobf::ironbrew_devirtualizer::devirtualizer_context>(entry_point.get());
		
		// optimizations
		devirtualizer_markers::mark_deserializer deserializer_marker(entry_point);
		deserializer_marker.optimize();

		std::cout << "start marker 2\n";
		devirtualizer_markers::mark_binary_functions binary_marker(entry_point);
		binary_marker.optimize();

		std::cout << "end marker 2\n";

		devirtualizer_markers::mark_wrapper wrapper_marker(entry_point);
		wrapper_marker.optimize();

		std::cout << "wrap\n";

		// todo mark deserializer get functions -> todo symbolic execute deserializer

		const auto deserializer_function = context->find_symbol("bytecode_deserializer");
		if (deserializer_function == nullptr)
			throw std::runtime_error("failed to find deserializer function");
		std::cout << "passed1\n";
		const auto vm_xor_key = context->find_symbol("vm_xor_key");
		if (vm_xor_key == nullptr)
			throw std::runtime_error("failed to find vm xor key symbol");
		std::cout << "passed2\n";
		const auto vm_string = context->find_symbol("vm_string");
		if (vm_string == nullptr)
			throw std::runtime_error("failed to find vm string symbol");
		std::cout << "passed3\n";
		const auto entry_point_bst = context->find_symbol("__entry_tree__");
		if (entry_point_bst == nullptr)
			throw std::runtime_error("failed to find entry point symbol");
		std::cout << "passed4\n";
		std::cout << entry_point_bst->symbol_value->to_string() << std::endl;

		//std::cout << devirtualizer_context->vm_string << std::endl;

		std::cout << "startdes\n";
		
		symbolic_execution::deserializer::deserializer_emulator_main deserializer_emulator(deserializer_function->symbol_value.get(), vm_string->symbol_value->to_string(), vm_xor_key->symbol_value->as<ir::expression::numeral_literal>()->value);
		auto new_chunk = deserializer_emulator.deserialize();

		const auto bst_if_root = static_cast<ir::statement::if_statement*>(entry_point_bst->symbol_value.get());
		opcode_identifiers::opcode_identifier_client identifier_client;

		std::cout << "searchtree:" << bst_if_root->to_string() << std::endl;;

		
		search_tree = std::make_unique<symbolic_execution::loop_unrolled_bst>(bst_if_root);

		static int invalidctr = 0;
		search_tree->callback_functor = [&](vm_arch::instruction& instruction, ir::statement::block* path) -> vm_arch::opcode {
			//std::cout << "opcode:" << static_cast<int>(instruction.get().op) << std::endl;
			if (instruction.op == vm_arch::opcode::op_invalid) { // didn't recongize the virtual yet.
				auto result = identifier_client.handle(path);
				std::cout << "res:" << vm_arch::opcode_map[result] << std::endl;
				instruction.op = result;
			}

			if (instruction.op == vm_arch::opcode::op_invalid && path != nullptr) {
				std::cout << "failed exception, info:" << std::endl;

				for (auto& stat : path->body) {
					std::cout << stat->to_string() << std::endl;
				}
				//std::cout << "hasret:" << path->find_first_of<ir::statement::do_block>()->get().body->ret.value()->to_string() << std::endl;
				
				std::cout << "backtrack:" << search_tree->back_track.size() << std::endl;
			}

			return instruction.op;
		};

		std::thread([this](deobf::vm_arch::proto* main_proto) {
			proccess_chunk(std::forward<deobf::vm_arch::proto*>(main_proto));
		}, new_chunk.get()).join();

		new_chunk->print_chunk();

		//tree.set_callback(std::mem_fn(&opcode_identifiers::opcode_identifier_client::handle));

		auto cfg_result = vm_arch::control_flow_graph::generate_graph(new_chunk.get()->instructions);

		static_chunk_analysis::test_spam::run(cfg_result->references);
		
		dynamic_chunk_analysis::run_analysis_session(new_chunk.get());
	}
}
