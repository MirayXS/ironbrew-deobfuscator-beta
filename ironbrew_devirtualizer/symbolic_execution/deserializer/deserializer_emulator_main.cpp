#include "deserializer_emulator_main.hpp"
#include "vm_arch/proto.hpp"
#include "deserializer_context.hpp"

namespace deobf::ironbrew_devirtualizer::symbolic_execution::deserializer {
	std::unique_ptr<vm_arch::proto> deserializer_emulator_main::deserialize() {
        auto proto = std::make_unique<vm_arch::proto>();

        for (const auto current_order : deserializer_ctx->chunk_order) {
            switch (current_order) {
                case process_order::constants: {
                    const auto kst_max_size = deserializer_helper_object->get_32_bits();
                    for (auto i = 0ul; i < kst_max_size; ++i) {
                        const auto randomized_order_type = deserializer_helper_object->get_8_bits();
                        if (const auto& kst_type = deserializer_ctx->constant_order_mapping.find(randomized_order_type); kst_type != deserializer_ctx->constant_order_mapping.cend()) {
                            switch (kst_type->second) {
                                case constant_type::string: {
                                    proto->constants.push_back(std::make_unique<vm_arch::constant>(deserializer_helper_object->get_string()));
                                    break;
                                }
                                case constant_type::number: {
                                    proto->constants.push_back(std::make_unique<vm_arch::constant>(deserializer_helper_object->get_float()));
                                    break;
                                }
                                case constant_type::boolean: {
                                    proto->constants.push_back(std::make_unique<vm_arch::constant>(static_cast<bool>(deserializer_helper_object->get_8_bits())));
                                    break;
                                }
                                default:
                                    throw std::runtime_error("unknown constant type? [deserializer]");
                            }
                        }
                        else {
                            proto->constants.push_back(std::make_unique<vm_arch::constant>(std::nullptr_t{ }));
                        }
                    }

                    break;
                }
                case process_order::instructions: {
                    // process instructions (not metamorphic)
                    const auto instruction_max_size = deserializer_helper_object->get_32_bits();

                    for (auto i = 0ul; i < instruction_max_size; ++i) {
                        const auto instruction_descriptor = deserializer_helper_object->get_8_bits();
                        if (deserializer_helper_object->get_bits(instruction_descriptor, 1, 1) == 0) {
                            const auto instruction_bitfield_1 = deserializer_helper_object->get_bits(instruction_descriptor, 2, 3);
                            const auto instruction_bitfield_2 = deserializer_helper_object->get_bits(instruction_descriptor, 4, 6);

                            // not passing it straight because undefined behaviour
                            const auto virtual_opcode = deserializer_helper_object->get_16_bits();
                            const auto new_instruction_a = deserializer_helper_object->get_16_bits();

                            auto new_instruction = std::make_unique<vm_arch::instruction>(virtual_opcode, new_instruction_a);

                            const auto instruction_type = static_cast<enum vm_arch::instruction_type>(instruction_bitfield_1);
                            new_instruction->type = instruction_type;

                            switch (instruction_type) {
                                case vm_arch::instruction_type::abc: {
                                    new_instruction->b = deserializer_helper_object->get_16_bits();
                                    new_instruction->c = deserializer_helper_object->get_16_bits();
                                    break;
                                }
                                case vm_arch::instruction_type::abx: {
                                    new_instruction->bx = deserializer_helper_object->get_32_bits();
                                    break;
                                }
                                case vm_arch::instruction_type::asbx: {
                                    new_instruction->sbx = deserializer_helper_object->get_32_bits() - std::numeric_limits<unsigned short>::max() - 1;
                                    break;
                                }
                                case vm_arch::instruction_type::asbxc: {
                                    new_instruction->sbx = deserializer_helper_object->get_32_bits() - std::numeric_limits<unsigned short>::max() - 1;
                                    new_instruction->c = deserializer_helper_object->get_16_bits();
                                    break;
                                }
                            }
                            

                            proto->instructions.push_back(std::move(new_instruction));
                            //std::cout << new_instruction->virtual_opcode << " " << new_instruction->a << " " << new_instruction->b << " " << new_instruction->c << std::endl;
                        }
                    }
                    break;
                }
                case process_order::lineinfo: {
                    const auto lineinfo_max_size = deserializer_helper_object->get_32_bits();
                    for (auto i = 0ul; i < lineinfo_max_size; ++i)
                        proto->lineinfo.push_back(deserializer_helper_object->get_32_bits());

                    break;
                }
                case process_order::protos: {
                    const auto proto_max_size = deserializer_helper_object->get_32_bits();
                    for (auto i = 0ul; i < proto_max_size; ++i)
                        proto->protos.emplace_back(deserialize());

                    break;
                }
                case process_order::parameters: {
                    proto->num_params = deserializer_helper_object->get_8_bits();
                    break;
                }
            }
        }

        return std::move(proto);
	}
}
