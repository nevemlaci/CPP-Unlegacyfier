//
// Created by nevemlaci on 2026. 09. 18..
//

#include "rewrite_code.hpp"

std::string rewrite_code(const std::string& code, UnlegacyfierConfig config) {
    std::map<std::string, clang::tooling::Replacements> raw_map;
    nl::ReplacementsMapWrapper wrapper(raw_map);

    UnlegacyfierActionFactory factory(config);
    FixItInterceptingConsumer consumer(wrapper);
    std::vector<std::string> const args = {"-std=c++17", "-fsyntax-only"};
    clang::tooling::FixedCompilationDatabase const comp_db(".", args);
    clang::tooling::ClangTool tool(comp_db, {"input.cpp"});
    tool.mapVirtualFile("input.cpp", code);
    tool.setDiagnosticConsumer(&consumer);

    auto exit_code = tool.run(&factory);

    if (exit_code != 0) {
        return "";
    }

    auto first_replacement_it = std::begin(raw_map);
    if (first_replacement_it == std::end(raw_map)) {
        return code;
    }

    auto rewritten = clang::tooling::applyAllReplacements(code, std::begin(raw_map)->second);
    if (rewritten) {
        return *rewritten;
    }

    llvm::consumeError(rewritten.takeError());
    return "";
}