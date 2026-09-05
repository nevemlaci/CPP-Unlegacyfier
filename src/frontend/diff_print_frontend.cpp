#include "frontend/diff_print_frontend.hpp"

#include <dtl/dtl.hpp>
#include <sstream>

std::vector<std::string> DiffPrintFrontend::split_lines(const std::string& str) {
    std::vector<std::string> lines;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }
    return lines;
}

DiffPrintFrontend::DiffPrintFrontend(nl::ReplacementMapRef replacement_map)
    : replacement_map(replacement_map) {}

void DiffPrintFrontend::print_diagnostics() {
    for (auto& [file, replacements] : replacement_map) {
        llvm::outs() << "Diagnostics for file: " << file << "\n\n";
        if (replacements.empty()) {
            continue;
        }
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> BufferOrErr =
            llvm::MemoryBuffer::getFile(file);

        if (!BufferOrErr) {
            llvm::errs() << "  [Error] Could not read file to calculate lines: " << file << '\n';
            continue;
        }
        std::string original_code = BufferOrErr.get()->getBuffer().str();
        auto rewritten_code_or_err =
            clang::tooling::applyAllReplacements(original_code, replacements);
        if (!rewritten_code_or_err) {
            llvm::consumeError(rewritten_code_or_err.takeError());
            continue;
        }
        std::string modified_code = *rewritten_code_or_err;

        std::vector<std::string> original_lines = split_lines(original_code);
        std::vector<std::string> modified_lines = split_lines(modified_code);

        dtl::Diff<std::string, std::vector<std::string>> diff(original_lines, modified_lines);
        diff.compose();
        diff.composeUnifiedHunks();

        std::cout << "--- " << file << "\n";
        std::cout << "+++ " << file << "\n";

        diff.printUnifiedFormat(std::cout);
        std::cout << "\n";
    }
}
