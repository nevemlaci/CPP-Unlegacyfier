#pragma once

#include "config/unlegacifier_config.h"

#include "clang/Tooling/Tooling.h"

#include <memory>
#include <util/types.h>

class UnlegacifierActionFactory : public clang::tooling::FrontendActionFactory {
    UnlegacifierConfig config;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    UnlegacifierActionFactory(
        UnlegacifierConfig config, nl::ReplacementMapRef shared_replacement_map
    );
    std::unique_ptr<clang::FrontendAction> create() override;
};