#pragma once

#include "config/unlegacyfier_config.hpp"

#include "clang/Tooling/Tooling.h"

#include <memory>
#include <util/types.hpp>

class UnlegacyfierActionFactory : public clang::tooling::FrontendActionFactory {
    UnlegacyfierConfig config;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    UnlegacyfierActionFactory(UnlegacyfierConfig config,
                              nl::ReplacementMapRef shared_replacement_map);
    std::unique_ptr<clang::FrontendAction> create() override;
};