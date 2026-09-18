//
// Created by nevemlaci on 2026. 09. 18..
//

#include <diagnostic_consumer.hpp>

FixItInterceptingConsumer::FixItInterceptingConsumer(nl::ReplacementsMapWrapper& map)
    : replacement_map(map), console_printer(llvm::errs(), diagnostic_options) {}

void FixItInterceptingConsumer::BeginSourceFile(const clang::LangOptions& LO,
                                                const clang::Preprocessor* PP) {
    console_printer.BeginSourceFile(LO, PP);
}

void FixItInterceptingConsumer::EndSourceFile() { console_printer.EndSourceFile(); }

void FixItInterceptingConsumer::HandleDiagnostic(clang::DiagnosticsEngine::Level level,
                                                 const clang::Diagnostic& info) {
    console_printer.HandleDiagnostic(level, info);

    for (unsigned i = 0; i < info.getNumFixItHints(); ++i) {
        const clang::FixItHint& hint = info.getFixItHint(i);
        if (hint.isNull() || !info.hasSourceManager()) {
            continue;
        }

        clang::tooling::Replacement rep(info.getSourceManager(), hint.RemoveRange,
                                        hint.CodeToInsert);

        replacement_map.add_replacement(rep);
    }
}