#pragma once

#include "util/types.hpp"

#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

class DiffPrintFrontend {
    nl::ReplacementMapRef replacement_map;

    std::array<unsigned, 2> get_file_position_from_byte_offset(unsigned offset,
                                                               llvm::StringRef content);

    std::vector<std::string> split_lines(const std::string& str);

  public:
    DiffPrintFrontend(nl::ReplacementMapRef replacement_map);

    void print_diagnostics();
};