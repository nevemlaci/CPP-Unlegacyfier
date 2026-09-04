#include "frontend/vscode_preview_frontend.hpp"

VsCodePreviewFrontend::VsCodePreviewFrontend(nl::ReplacementMapRef replacement_map)
    : replacement_map(replacement_map) {}

void VsCodePreviewFrontend::generate_preview() {
    llvm::json::Array json_manifest;

    for (const auto& [filepath, replacements] : replacement_map) {
        if (replacements.empty()) {
            continue;
        }

        auto buffer = llvm::MemoryBuffer::getFile(filepath);
        if (!buffer) {
            continue;
        }
        llvm::StringRef original_code = buffer.get()->getBuffer();
        auto rewritten_code_or_err =
            clang::tooling::applyAllReplacements(original_code, replacements);
        if (!rewritten_code_or_err) {
            llvm::consumeError(rewritten_code_or_err.takeError());
            continue;
        }
        llvm::SmallString<128> temp_path;
        int temp_fd;
        std::error_code ec =
            llvm::sys::fs::createTemporaryFile("unlegacy_preview", "cpp", temp_fd, temp_path);

        if (!ec) {
            llvm::raw_fd_ostream temp_out(temp_fd, true);
            temp_out << *rewritten_code_or_err;
            temp_out.close();

            json_manifest.push_back(
                llvm::json::Object{{"original", filepath}, {"modified", temp_path.str().str()}});
        }

        llvm::outs() << llvm::json::Value(std::move(json_manifest)) << '\n';
    }
}
