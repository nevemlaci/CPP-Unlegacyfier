#include "transformer/transformer.hpp"

Transformer::Transformer(clang::ASTContext& context) : context(context) {}

clang::DiagnosticBuilder Transformer::create_diagnostic(const std::string& desc,
                                                        clang::SourceLocation loc,
                                                        clang::diag::Severity severity) {

    clang::DiagnosticIDs::CustomDiagDesc diag_desc(severity, desc);

    auto& diagnostics_engine = context.getDiagnostics();

    auto diag_id = diagnostics_engine.getDiagnosticIDs()->getCustomDiagID(diag_desc);

    return diagnostics_engine.Report(loc, diag_id);
}