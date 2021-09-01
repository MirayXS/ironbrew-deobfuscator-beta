#pragma once
#include "../../vm_arch/proto.hpp"

namespace deobf::ironbrew_devirtualizer::dynamic_chunk_analysis {
	extern void run_analysis_session(vm_arch::proto* current_proto);
}