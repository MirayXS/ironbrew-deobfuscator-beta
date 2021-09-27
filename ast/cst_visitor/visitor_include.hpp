// visitor pattern dependancy ( this is used to convert the CST into our AST. )
#pragma once

#include <LuaBaseVisitor.h>
#include "ast/ir/statement.hpp"
#include "ast/ir/expression.hpp"

// semantic analysis (done for functions, function parameters, variables) is always impelemented via visitor.

// todo fix precedence for function calls? (i.e. the following expression a()() might result in unexpected behaviour), ANTLR grammar issue?

// TODO RENAME FORLOOP VARIABLES

// todo fix multi_statements on visitor, unordered somewhat?

namespace deobf::ast {
	struct cst_visitor final : public LuaBaseVisitor {
	private:
		static std::size_t function_counter, parameter_counter, variable_counter; // used for semantic analysis
		ir::statement::block* current_parse_block = nullptr;

		// todo enter/close scope functions (so we wont have to do the bullshit as we did on cst_visitor::visitFuncbody)
	public:
		virtual antlrcpp::Any visitChunk(LuaParser::ChunkContext* ctx) override;
		virtual antlrcpp::Any visitBlock(LuaParser::BlockContext* ctx) override;
		virtual antlrcpp::Any visitTableconstructor(LuaParser::TableconstructorContext* ctx) override;
		virtual antlrcpp::Any visitWhileStat(LuaParser::WhileStatContext* ctx) override;
		virtual antlrcpp::Any visitStat(LuaParser::StatContext* ctx) override;
		virtual antlrcpp::Any visitForInStat(LuaParser::ForInStatContext* ctx) override;
		virtual antlrcpp::Any visitForStat(LuaParser::ForStatContext* ctx) override;
		virtual antlrcpp::Any visitFunctioncall(LuaParser::FunctioncallContext* ctx) override;
		virtual antlrcpp::Any visitFunctiondef(LuaParser::FunctiondefContext* ctx) override;
		virtual antlrcpp::Any visitFuncbody(LuaParser::FuncbodyContext* ctx) override;
		virtual antlrcpp::Any visitFuncStat(LuaParser::FuncStatContext* ctx) override;
		virtual antlrcpp::Any visitVarDecl(LuaParser::VarDeclContext* ctx) override;
		virtual antlrcpp::Any visitVarSuffix(LuaParser::VarSuffixContext* ctx) override;
		virtual antlrcpp::Any visitVar(LuaParser::VarContext* ctx) override;
		virtual antlrcpp::Any visitVarlist(LuaParser::VarlistContext* ctx) override;
		virtual antlrcpp::Any visitRetstat(LuaParser::RetstatContext* ctx) override;
		virtual antlrcpp::Any visitNamelist(LuaParser::NamelistContext* ctx) override;
		virtual antlrcpp::Any visitLocalVarDecl(LuaParser::LocalVarDeclContext* ctx) override;
		virtual antlrcpp::Any visitLocalFuncStat(LuaParser::LocalFuncStatContext* ctx) override;
		virtual antlrcpp::Any visitRepeatStat(LuaParser::RepeatStatContext* ctx) override;
		virtual antlrcpp::Any visitIfStat(LuaParser::IfStatContext* ctx) override;
		virtual antlrcpp::Any visitNameAndArgs(LuaParser::NameAndArgsContext* ctx) override;
		virtual antlrcpp::Any visitArgs(LuaParser::ArgsContext* ctx) override;
		virtual antlrcpp::Any visitExplist(LuaParser::ExplistContext* ctx) override;
		virtual antlrcpp::Any visitString(LuaParser::StringContext* ctx) override;
		virtual antlrcpp::Any visitDoStat(LuaParser::DoStatContext* ctx) override;
		virtual antlrcpp::Any visitPrefixexp(LuaParser::PrefixexpContext* ctx) override;
		virtual antlrcpp::Any visitExp(LuaParser::ExpContext* ctx) override;
	};
}
