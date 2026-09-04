#include "frontend/unlegacifier_consumer.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendAction.h"

#include <clang/Frontend/CompilerInstance.h>
#include <frontend/unlegacifier_frontend.h>

UnlegacifierFrontend::UnlegacifierFrontend(
    UnlegacifierConfig config, nl::ReplacementMapRef shared_replacement_map
)
    : config(config), shared_replacement_map(shared_replacement_map) {}

std::unique_ptr<clang::ASTConsumer>
UnlegacifierFrontend::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) {
    return std::make_unique<UnlegacifierConsumer>(
        compiler.getASTContext(), config, shared_replacement_map
    );
}