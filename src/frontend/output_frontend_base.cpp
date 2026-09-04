#include "frontend/output_frontend_base.hpp"

#include "util/types.h"

#pragma once

class OutputFrontendBase {
    nl::ReplacementMapRef replacement_map;

  public:
    OutputFrontendBase(nl::ReplacementMapRef replacement_map) : replacement_map(replacement_map) {}

    virtual void run() {}
};