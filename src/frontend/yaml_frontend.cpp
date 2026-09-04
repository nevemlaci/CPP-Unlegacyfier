#include "frontend/yaml_frontend.hpp"

YamlFrontend::YamlFrontend(nl::ReplacementMapRef replacement_map)
    : replacement_map(replacement_map) {}

void YamlFrontend::export_to_yaml(const std::string& output_filename) {
    clang::tooling::TranslationUnitReplacements tu_replacements;
    tu_replacements.MainSourceFile = "";

    for (const auto& [file, replacements] : replacement_map) {
        for (const auto& replacement : replacements) {
            tu_replacements.Replacements.push_back(replacement);
        }
    }

    std::error_code ec;
    llvm::raw_fd_ostream out_stream(output_filename, ec, llvm::sys::fs::OF_None);

    if (ec) {
        llvm::errs() << "  [Error] Failed to open YAML output file: " << ec.message() << '\n';
        return;
    }

    llvm::yaml::Output yaml_out(out_stream);
    yaml_out << tu_replacements;

    llvm::outs() << "Successfully exported fixes to " << output_filename << '\n';
}
