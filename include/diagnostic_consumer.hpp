//
// Created by nevemlaci on 2026. 09. 08..
//

#ifndef CPP_UNLEGACYFIER_DIAGNOSTIC_CONSUMER_HPP
#define CPP_UNLEGACYFIER_DIAGNOSTIC_CONSUMER_HPP

#include "util/replacement_map.hpp"

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/Core/Replacement.h>

namespace nl {
class ReplacementsMapWrapper;
}

class FixItInterceptingConsumer : public clang::DiagnosticConsumer {
  public:
    FixItInterceptingConsumer(nl::ReplacementsMapWrapper& map);

    void BeginSourceFile(const clang::LangOptions& LO, const clang::Preprocessor* PP) override;

    void EndSourceFile() override;

    void HandleDiagnostic(clang::DiagnosticsEngine::Level level,
                          const clang::Diagnostic& info) override;

  private:
    clang::DiagnosticOptions diagnostic_options;
    nl::ReplacementsMapWrapper& replacement_map;
    clang::TextDiagnosticPrinter console_printer;
};

#endif // CPP_UNLEGACYFIER_DIAGNOSTIC_CONSUMER_HPP
