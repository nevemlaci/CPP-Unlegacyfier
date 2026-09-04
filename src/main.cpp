#include "frontend/diff_print_frontend.hpp"
#include "frontend/unlegacyfier_factory.hpp"
#include "frontend/vscode_preview_frontend.hpp"
#include "frontend/yaml_frontend.hpp"
#include "util/replacement_map.hpp"

#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Refactoring.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

enum FrontendEnum { DiffPrint, Yaml, VsCode };

// clang-format off
static llvm::cl::OptionCategory ToolCategory("C++ Unlegacyfier");

static llvm::cl::opt<bool> OptEnumFixer(
    "enum-class-fixer", 
    llvm::cl::desc("Replaces unscoped enums with enum classes."),
    llvm::cl::cat(ToolCategory)
);
static llvm::cl::opt<bool> OptBeginEnd(
    "begin-end-fixer",
    llvm::cl::desc("Replace .begin() and .end() with std::begin() and std::end()"),
    llvm::cl::cat(ToolCategory)
);
static llvm::cl::opt<bool> OptShorterFunctor(
    "shorter-functor",
    llvm::cl::desc("Replace long functor objects ( e.g. std::greater<T>() ) with a shorter version ( e.g. std::greater<>() )"),
    llvm::cl::cat(ToolCategory)
);
static llvm::cl::opt<bool> OptAddMissingOverride(
    "add-missing-override",
    llvm::cl::desc("Adds override to member functions that are overrides but missing the specifier."),
    llvm::cl::cat(ToolCategory)
);

static llvm::cl::opt<bool> OptReplaceTypedefWithUsing(
    "replace-typedef-using",
    llvm::cl::desc("Replaces type aliases created by typedef with the using directive equivalent."),
    llvm::cl::cat(ToolCategory)
);

static llvm::cl::list<FrontendEnum> Frontends(
    "frontends", 
    llvm::cl::desc("Set enabled frontends."),
    llvm::cl::values(
        clEnumValN(DiffPrint, "diff", "Print all changes suggested by the tool in the diff format."),
        clEnumValN(Yaml, "yaml", "Print all changes suggested by the tool in the clang yaml format."),
        clEnumValN(VsCode, "vscode", "Print all changes suggested by the tool in the vscode.diff JSON format.")
    ),
    llvm::cl::CommaSeparated,
    llvm::cl::cat(ToolCategory)
);
// clang-format on

int main(int argc, const char** argv) {
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, ToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    clang::tooling::CommonOptionsParser& optionsParser = ExpectedParser.get();
    clang::tooling::RefactoringTool tool(optionsParser.getCompilations(),
                                         optionsParser.getSourcePathList());
    auto& map = tool.getReplacements();
    nl::ReplacementsMapWrapper shared_replacement_map(map);
    UnlegacyfierConfig config{OptEnumFixer, OptBeginEnd, OptShorterFunctor, OptAddMissingOverride,
                              OptReplaceTypedefWithUsing};

    tool.appendArgumentsAdjuster(clang::tooling::getInsertArgumentAdjuster(
        "-w", clang::tooling::ArgumentInsertPosition::BEGIN));
    tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(
        "-resource-dir=" CLANG_RESOURCE_DIR, clang::tooling::ArgumentInsertPosition::BEGIN));

    UnlegacyfierActionFactory factory(config, shared_replacement_map);

    auto result = tool.run(&factory);
    DiffPrintFrontend diff_print_frontend(shared_replacement_map);
    YamlFrontend yaml_frontend(shared_replacement_map);
    VsCodePreviewFrontend vscode_frontend(shared_replacement_map);
    for (auto frontend : Frontends) {
        switch (frontend) {
            case DiffPrint:
                diff_print_frontend.print_diagnostics();
                break;
            case Yaml:
                yaml_frontend.export_to_yaml("fixes.yaml");
                break;
            case VsCode:
                vscode_frontend.generate_preview();
                break;
        }
    }
    llvm::outs() << "\n";
    return result;
}