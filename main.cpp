#include <iostream>

// include directive : ../generated_files
#include "LuaLexer.h"
#include "LuaParser.h"
//#include "parser_combinator/parser_combinator.hpp"
#include <optional>
#include <chrono>

#include "ast/cst_visitor/visitor_include.hpp"

#include "optimization_passes/base_optimizer.hpp"
#include "optimization_passes/constant_folding.hpp"
#include "optimization_passes/constant_propagation.hpp"

#include "ironbrew_devirtualizer/devirtualizer_main.hpp"

using namespace antlr4; // yeah idc


// todo test driven development?

int main(int argc, char** argv) {
    std::ifstream file_stream("D:\\ironbrew-2-master\\IronBrew2 CLI\\bin\\Debug\\netcoreapp3.1\\out.lua");

    // proccess
    ANTLRInputStream input(file_stream);
    LuaLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    LuaParser parser(&tokens);
    LuaParser::ChunkContext* chunks = parser.chunk();
    std::cout << "done parsing\n";

    if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cerr << "can't load ast, syntax error" << std::endl;
        return 0;
    }
    //ast_visitor visitor;

    using namespace deobf::ast::ir;

    auto ast_tree = deobf::ast::cst_visitor{ }.visitChunk(chunks).as<std::shared_ptr<statement::block>>();
    
    std::cout << "done cst_visitor\n";

    // enter passes

    std::cout << "pass propagator\n";

    deobf::optimization_passes::constant_propagation propagator(ast_tree.get());
    propagator.optimize();

    std::cout << "pass folder\n";
    deobf::optimization_passes::constant_folding folder(ast_tree.get());
    folder.optimize();

    // passes done

    //std::cout << ast_tree->find_first_of<statement::do_block>().value()->body->body.at(0)->to_string() << std::endl;
    
    //std::cout << ast_tree->find_first_of<expression::function_call>().value()->to_string() << std::endl;
    std::cout << "devirtualizer on\n";
    deobf::ironbrew_devirtualizer::ironbrew_devirtualizer devirtualizer_job(ast_tree.get());
    devirtualizer_job.devirtualize();

   // std::cout << ast_tree->ret.value()->to_string();
    //auto nn = std::static_pointer_cast<ast_function_call>(ast_tree->statement_list.back())->name_and_args;
    //std::cout << std::static_pointer_cast<ast_string>(nn.back()->expr_args)->value << std::endl;

    //file_stream.close();
    
    std::getchar();
    return 0;
}
