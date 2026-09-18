//
// Created by nevemlaci on 2026. 09. 18..
//

#ifndef CPP_UNLEGACYFIER_REWRITE_CODE_HPP
#define CPP_UNLEGACYFIER_REWRITE_CODE_HPP
#include "config/unlegacyfier_config.hpp"
#include "diagnostic_consumer.hpp"
#include "frontend/unlegacyfier_factory.hpp"

#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Core/Replacement.h>
#include <string>

std::string rewrite_code(const std::string& code, UnlegacyfierConfig config);

#endif // CPP_UNLEGACYFIER_REWRITE_CODE_HPP
