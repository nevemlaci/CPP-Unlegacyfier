#pragma once

#include "util/types.h"

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>

class VsCodePreviewFrontend {
    nl::ReplacementMapRef replacement_map;

  public:
    VsCodePreviewFrontend(nl::ReplacementMapRef replacement_map);

    void generate_preview();
};